// Copyright 2022 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>
#include <stddef.h>
#include <math.h>

#include <arm_neon.h>

#include <xnnpack/common.h>
#include <xnnpack/math.h>
#include <xnnpack/math-stubs.h>


void xnn_math_f32_tanh__aarch64_neonfma_rr1_p6_div(
    size_t n,
    const float* input,
    float* output)
{
  assert(n % sizeof(float32x4_t) == 0);

  // Large number such that ulp(magic bias) == 0.5 and magic bias === 63.5 mod 2**21.
  const float32x4_t vmagic_bias = vmovq_n_f32(0x1.8000FEp+22f);
  const float32x4_t vminus_log2e = vmovq_n_f32(-0x1.715476p+0f);
  const float32x4_t vln2 = vmovq_n_f32(0x1.62E430p-1f);
  // Coefficient of polynomial approximation
  //   exp(-2t) - 1 ~ -2 * (t * (1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6))))))
  // on [-log(2)/2, log(2)/2]
  const float32x4_t vc6 = vmovq_n_f32(-0x1.6b7338p-5f);
  const float32x4_t vc5 = vmovq_n_f32(0x1.12278Ep-3f);
  const float32x4_t vc4 = vmovq_n_f32(-0x1.555716p-2f);
  const float32x4_t vc3 = vmovq_n_f32(0x1.5554B0p-1f);
  const float32x4_t vc2 = vmovq_n_f32(-0x1.FFFFFEp-1f);
  const float32x4_t vone = vmovq_n_f32(1.0f);
  const float32x4_t vtwo = vmovq_n_f32(2.0f);
  // The largest z for which tanhf(-z) is not saturated at -1.0f.
  const float32x4_t vsat_cutoff = vmovq_n_f32(0x1.205966p+3f);
  // Mask for the sign bit.
  const uint32x4_t vsign_mask = vmovq_n_u32(UINT32_C(0x80000000));

  for (; n != 0; n -= 4 * sizeof(float)) {
    const float32x4_t vx = vld1q_f32(input); input += 4;

    // General structure of the algorithm:
    //
    //           / expm1(2x) / (2 + expm1(2x)) if x <= 0
    //   f[x] :=
    //           \ -f[-x] if x >= 0
    //
    // First we compute f[-z] := expm1(-2z) / (2 + expm1(-2z)) where z = abs(x),
    // then replace result with -f[-z] if x >= 0.
    const float32x4_t vz = vabsq_f32(vx);

    // Compute reduced argument n := round(-z / log(2), 1).
    // We do it by adding a large number (magic bias), which cause rounding of the result to integer, then subtracing
    // the large number back. The trick with adding large number is valid only within certain bounds
    // (|-z / log(2)| <= 2**21, i.e. |z| <= 0x1.62E43p+20 = 1453635.0), but that is acceptable, because inputs x
    // outside of [-9.010913, 9.010913] (i.e. z outsize [0, 9.010913]) saturate tanhf(x). We fixup the result for such
    // inputs at the very end of the algorithm.
    // Note that addition-subtraction of the large number doesn't cause overflow for inputs in this range.
    float32x4_t vn = vfmaq_f32(vmagic_bias, vz, vminus_log2e);

    // Create a floating-point number s (scale) such that s == 2**(2n) for inputs which don't cause underflow, i.e.
    // 0 <= z <= 9.010913, and -13 <= n <= 0 accordingly.
    const float32x4_t vs = vreinterpretq_f32_s32(vshlq_n_s32(vreinterpretq_s32_f32(vn), 23));

    // Subtract the large number back to get final n := round(-z / log(2), 1) as a floating-point number.
    vn = vsubq_f32(vn, vmagic_bias);

    // Compute reduced argument t := z + n * log(2). Note that -t = -z - n * log(2).
    float32x4_t vt = vfmaq_f32(vz, vn, vln2);

    // Compute degree-6 polynomial approximation for exp(-2t) - 1 on [-log(2)/4, log(2)/4].
    //   P(-2t) = t * (1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))))
    //          = t + t * (t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))))
    //          = -2 * (t + t * p)
    float32x4_t vp = vfmaq_f32(vc5, vc6, vt);
    vp = vfmaq_f32(vc4, vp, vt);
    vp = vfmaq_f32(vc3, vp, vt);
    vp = vfmaq_f32(vc2, vp, vt);
    vp = vmulq_f32(vp, vt);

    // Reconstruct the exp(x) - 1 value:
    //   exp(x) - 1 = s * (1 - 2t * (1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))))) - 1
    //              = (s - 1) + s * (-2t) * (t + t * p)
    //              = (s - 1) - 2 * ((t * s) + (t * s) * p)
    vt = vmulq_f32(vt, vs);
    const float32x4_t vsm1 = vsubq_f32(vs, vone);
    vp = vfmaq_f32(vt, vp, vt);
    const float32x4_t vem1 = vfmsq_f32(vsm1, vtwo, vp);

    // Reconstruct tanh(-z) := expm1(-2z) / (2 + expm1(-2z))
    const float32x4_t vep1 = vaddq_f32(vem1, vtwo);
    float32x4_t vabsy = vdivq_f32(vem1, vep1);

    // The function saturates at +-1 for large inputs: tanhf(z) == +-1.0f for z > sat_cutoff ~= 9.010913.
    // Note that we use 1.0f, because sign will be copied from the input right after.
    const uint32x4_t vsat_mask = vcgtq_f32(vz, vsat_cutoff);
    vabsy = vbslq_f32(vsat_mask, vone, vabsy);

    // Reconstruct tanh[x] = sign(x) * tanh[-abs(x)]
    const float32x4_t vy = vbslq_f32(vsign_mask, vx, vabsy);

    vst1q_f32(output, vy); output += 4;
  }
}
