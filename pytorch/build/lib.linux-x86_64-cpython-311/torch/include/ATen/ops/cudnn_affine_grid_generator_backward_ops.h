#pragma once

// @generated by torchgen/gen.py from Operator.h

#include <tuple>
#include <vector>

// Forward declarations of any types needed in the operator signatures.
// We can't directly include these classes because it will cause circular include dependencies.
// This file is included by TensorBody.h, which defines the Tensor class.
#include <ATen/core/ATen_fwd.h>

namespace at {
namespace _ops {


struct TORCH_API cudnn_affine_grid_generator_backward {
  using schema = at::Tensor (const at::Tensor &, int64_t, int64_t, int64_t, int64_t);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::cudnn_affine_grid_generator_backward")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "cudnn_affine_grid_generator_backward(Tensor grad, int N, int C, int H, int W) -> Tensor grad_theta")
  static at::Tensor call(const at::Tensor & grad, int64_t N, int64_t C, int64_t H, int64_t W);
  static at::Tensor redispatch(c10::DispatchKeySet dispatchKeySet, const at::Tensor & grad, int64_t N, int64_t C, int64_t H, int64_t W);
};

struct TORCH_API cudnn_affine_grid_generator_backward_out {
  using schema = at::Tensor & (const at::Tensor &, int64_t, int64_t, int64_t, int64_t, at::Tensor &);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::cudnn_affine_grid_generator_backward")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "out")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "cudnn_affine_grid_generator_backward.out(Tensor grad, int N, int C, int H, int W, *, Tensor(a!) out) -> Tensor(a!)")
  static at::Tensor & call(const at::Tensor & grad, int64_t N, int64_t C, int64_t H, int64_t W, at::Tensor & out);
  static at::Tensor & redispatch(c10::DispatchKeySet dispatchKeySet, const at::Tensor & grad, int64_t N, int64_t C, int64_t H, int64_t W, at::Tensor & out);
};

}} // namespace at::_ops