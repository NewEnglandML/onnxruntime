// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "identity_op.h"

namespace onnxruntime {
namespace cuda {
ONNX_OPERATOR_KERNEL_EX(
    Dropout,
    kOnnxDomain,
    7,
    kCudaExecutionProvider,
    KernelDefBuilder()
        .TypeConstraint("T", {DataTypeImpl::GetTensorType<MLFloat16>(), DataTypeImpl::GetTensorType<float>(), DataTypeImpl::GetTensorType<double>()})
        .Alias(0, 0),
    IdentityOp<true>);

ONNX_OPERATOR_KERNEL_EX(
    Identity,
    kOnnxDomain,
    1,
    kCudaExecutionProvider,
    KernelDefBuilder()
        .TypeConstraint("T", DataTypeImpl::AllFixedSizeTensorTypes())
        .Alias(0, 0),
    IdentityOp<false>);
}  // namespace cuda
}  // namespace onnxruntime
