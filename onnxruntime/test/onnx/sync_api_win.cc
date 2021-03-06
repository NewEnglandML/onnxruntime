// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "sync_api.h"
#include <core/common/common.h>

using ::onnxruntime::common::Status;

Status CreateAndSubmitThreadpoolWork(ONNXRUNTIME_CALLBACK_FUNCTION callback, void* data, PThreadPool pool) {
  PTP_WORK work = CreateThreadpoolWork(callback, data, pool);
  if (!work) {
    return Status(::onnxruntime::common::ONNXRUNTIME, ::onnxruntime::common::FAIL, "create thread pool task failed");
  }
  SubmitThreadpoolWork(work);
  return Status::OK();
}

Status WaitAndCloseEvent(ONNXRUNTIME_EVENT finish_event) {
  DWORD dwWaitResult = WaitForSingleObject(finish_event, INFINITE);
  (void)CloseHandle(finish_event);
  if (dwWaitResult != WAIT_OBJECT_0) {
    return ONNXRUNTIME_MAKE_STATUS(ONNXRUNTIME, FAIL, "WaitForSingleObject failed");
  }
  return Status::OK();
}

Status CreateOnnxRuntimeEvent(ONNXRUNTIME_EVENT* out) {
  if (out == nullptr)
    return Status(::onnxruntime::common::ONNXRUNTIME, ::onnxruntime::common::INVALID_ARGUMENT, "");
  HANDLE finish_event = CreateEvent(
      NULL,   // default security attributes
      TRUE,   // manual-reset event
      FALSE,  // initial state is nonsignaled
      NULL);
  if (finish_event == NULL) {
    return ONNXRUNTIME_MAKE_STATUS(ONNXRUNTIME, FAIL, "unable to create finish event");
  }
  *out = finish_event;
  return Status::OK();
}

Status OnnxRuntimeSetEventWhenCallbackReturns(ONNXRUNTIME_CALLBACK_INSTANCE pci, ONNXRUNTIME_EVENT finish_event) {
  if (finish_event == nullptr)
    return Status(::onnxruntime::common::ONNXRUNTIME, ::onnxruntime::common::INVALID_ARGUMENT, "");
  if (pci)
    SetEventWhenCallbackReturns(pci, finish_event);
  else if (!SetEvent(finish_event)) {
    return ONNXRUNTIME_MAKE_STATUS(ONNXRUNTIME, FAIL, "SetEvent failed");
  }
  return Status::OK();
}

void ONNXRuntimeCloseEvent(ONNXRUNTIME_EVENT finish_event) {
  (void)CloseHandle(finish_event);
}