// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SHELL_PLATFORM_ANDROID_PLATFORM_VIEW_ANDROID_H_
#define SHELL_PLATFORM_ANDROID_PLATFORM_VIEW_ANDROID_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "flutter/fml/platform/android/jni_weak_ref.h"
#include "flutter/fml/platform/android/scoped_java_ref.h"
#include "flutter/lib/ui/window/platform_message.h"
#include "flutter/shell/common/platform_view.h"
#include "flutter/shell/platform/android/android_native_window.h"
#include "flutter/shell/platform/android/android_surface.h"
#include "lib/fxl/memory/weak_ptr.h"

namespace shell {

class PlatformViewAndroid final : public PlatformView {
 public:
  static bool Register(JNIEnv* env);

  PlatformViewAndroid(PlatformView::Delegate& delegate,
                      blink::TaskRunners task_runners,
                      fml::jni::JavaObjectWeakGlobalRef java_object,
                      bool use_software_rendering);

  ~PlatformViewAndroid() override;

  void NotifyCreated(fxl::RefPtr<AndroidNativeWindow> native_window);

  void NotifyChanged(const SkISize& size);

  // |shell::PlatformView|
  void NotifyDestroyed() override;

  void DispatchPlatformMessage(JNIEnv* env,
                               std::string name,
                               jobject message_data,
                               jint message_position,
                               jint response_id);

  void DispatchEmptyPlatformMessage(JNIEnv* env,
                                    std::string name,
                                    jint response_id);

  void InvokePlatformMessageResponseCallback(JNIEnv* env,
                                             jint response_id,
                                             jobject java_response_data,
                                             jint java_response_position);

  void InvokePlatformMessageEmptyResponseCallback(JNIEnv* env,
                                                  jint response_id);

  void DispatchSemanticsAction(JNIEnv* env,
                               jint id,
                               jint action,
                               jobject args,
                               jint args_position);

  void RegisterExternalTexture(
      int64_t texture_id,
      const fml::jni::JavaObjectWeakGlobalRef& surface_texture);

 private:
  const fml::jni::JavaObjectWeakGlobalRef java_object_;
  const std::unique_ptr<AndroidSurface> android_surface_;
  // We use id 0 to mean that no response is expected.
  int next_response_id_ = 1;
  std::unordered_map<int, fxl::RefPtr<blink::PlatformMessageResponse>>
      pending_responses_;

  // |shell::PlatformView|
  void UpdateSemantics(blink::SemanticsNodeUpdates update) override;

  // |shell::PlatformView|
  void HandlePlatformMessage(
      fxl::RefPtr<blink::PlatformMessage> message) override;

  // |shell::PlatformView|
  std::unique_ptr<VsyncWaiter> CreateVSyncWaiter() override;

  // |shell::PlatformView|
  std::unique_ptr<Surface> CreateRenderingSurface() override;

  // |shell::PlatformView|
  sk_sp<GrContext> CreateResourceContext() const override;

  void InstallFirstFrameCallback();

  void FireFirstFrameCallback();

  FXL_DISALLOW_COPY_AND_ASSIGN(PlatformViewAndroid);
};

}  // namespace shell

#endif  // SHELL_PLATFORM_ANDROID_PLATFORM_VIEW_ANDROID_H_
