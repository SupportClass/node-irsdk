#pragma once

#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace NodeIrSdk {

  IRSDKWrapper irsdk;

  Napi::Value start(const Napi::CallbackInfo&);

  Napi::Value shutdown(const Napi::CallbackInfo&);

  Napi::Value isInitialized(const Napi::CallbackInfo&);

  Napi::Value isConnected(const Napi::CallbackInfo&);

  Napi::Value updateSessionInfo(const Napi::CallbackInfo&);

  Napi::Value getSessionInfo(const Napi::CallbackInfo&);

  Napi::Value updateTelemetry(const Napi::CallbackInfo&);

  Napi::Value getTelemetry(const Napi::CallbackInfo&);
  
  Napi::Value getTelemetryDescription(const Napi::CallbackInfo&);
  
  void sendCmd(const Napi::CallbackInfo& info);

  static void cleanUp(void* arg);

  // this defines public api of native addon
  Napi::Object init(Napi::Env env, Napi::Object exports)
  {
    irsdk.startup();

    napi_add_env_cleanup_hook(env, cleanUp, nullptr);

    exports.Set("start", Napi::Function::New(env, start));
    exports.Set("shutdown", Napi::Function::New(env, shutdown));

    exports.Set("isInitialized", Napi::Function::New(env, isInitialized));
    exports.Set("isConnected", Napi::Function::New(env, isConnected));

    exports.Set("updateSessionInfo", Napi::Function::New(env, updateSessionInfo));
    exports.Set("getSessionInfo", Napi::Function::New(env, getSessionInfo));

    exports.Set("updateTelemetry", Napi::Function::New(env, updateTelemetry));
    exports.Set("getTelemetryDescription", Napi::Function::New(env, getTelemetryDescription));
    exports.Set("getTelemetry", Napi::Function::New(env, getTelemetry));
    
    exports.Set("sendCmd", Napi::Function::New(env, sendCmd));

    return exports;
  }

  // name of native addon
  NODE_API_MODULE(IrSdkNodeBindings, init)
}