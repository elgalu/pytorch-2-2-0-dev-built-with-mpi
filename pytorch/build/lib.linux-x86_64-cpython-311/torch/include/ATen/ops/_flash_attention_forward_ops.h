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


struct TORCH_API _flash_attention_forward {
  using schema = ::std::tuple<at::Tensor,at::Tensor,at::Tensor,at::Tensor,at::Tensor> (const at::Tensor &, const at::Tensor &, const at::Tensor &, const c10::optional<at::Tensor> &, const c10::optional<at::Tensor> &, c10::optional<c10::SymInt>, c10::optional<c10::SymInt>, double, bool, bool, c10::optional<double>);
  using ptr_schema = schema*;
  // See Note [static constexpr char* members for windows NVCC]
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(name, "aten::_flash_attention_forward")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(overload_name, "")
  STATIC_CONSTEXPR_STR_INL_EXCEPT_WIN_CUDA(schema_str, "_flash_attention_forward(Tensor query, Tensor key, Tensor value, Tensor? cum_seq_q, Tensor? cum_seq_k, SymInt? max_q, SymInt? max_k, float dropout_p, bool is_causal, bool return_debug_mask, *, float? scale=None) -> (Tensor output, Tensor softmax_logsumexp, Tensor philox_seed, Tensor philox_offset, Tensor debug_attn_mask)")
  static ::std::tuple<at::Tensor,at::Tensor,at::Tensor,at::Tensor,at::Tensor> call(const at::Tensor & query, const at::Tensor & key, const at::Tensor & value, const c10::optional<at::Tensor> & cum_seq_q, const c10::optional<at::Tensor> & cum_seq_k, c10::optional<c10::SymInt> max_q, c10::optional<c10::SymInt> max_k, double dropout_p, bool is_causal, bool return_debug_mask, c10::optional<double> scale);
  static ::std::tuple<at::Tensor,at::Tensor,at::Tensor,at::Tensor,at::Tensor> redispatch(c10::DispatchKeySet dispatchKeySet, const at::Tensor & query, const at::Tensor & key, const at::Tensor & value, const c10::optional<at::Tensor> & cum_seq_q, const c10::optional<at::Tensor> & cum_seq_k, c10::optional<c10::SymInt> max_q, c10::optional<c10::SymInt> max_k, double dropout_p, bool is_causal, bool return_debug_mask, c10::optional<double> scale);
};

}} // namespace at::_ops