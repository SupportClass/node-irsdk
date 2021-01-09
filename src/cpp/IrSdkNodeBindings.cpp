#pragma once

#include "IRSDKWrapper.h"
#include "IrSdkNodeBindings.h"
#include "IrSdkBindingHelpers.h"
#include "IrSdkCommand.h"

#include <iostream>
#include <stdint.h>

using namespace Napi;

namespace NodeIrSdk
{

Napi::Value start(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  return Napi::Boolean::New(env, irsdk.startup());
}

Napi::Value shutdown(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  irsdk.shutdown();
  return env.Undefined();
}

Napi::Value isInitialized(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  return Napi::Boolean::New(env, irsdk.isInitialized());
}

Napi::Value isConnected(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  return Napi::Boolean::New(env, irsdk.isConnected());
}

Napi::Value updateSessionInfo(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  return Napi::Boolean::New(env, irsdk.updateSessionInfo());
}

Napi::Value getSessionInfo(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  return Napi::String::New(env, irsdk.getSessionInfo());
}

Napi::Value updateTelemetry(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  return Napi::Boolean::New(env, irsdk.updateTelemetry());
}

Napi::Value getTelemetry(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  Napi::Object rootObj = Napi::Object::New(env);
  Napi::Object valuesObj = Napi::Object::New(env);
  rootObj.Set("timestamp", Napi::Date::New(env, irsdk.getLastTelemetryUpdateTS()));

  std::vector<irsdk_varHeader *> headers = irsdk.getVarHeaders();

  for (const auto item : headers)
  {
    IRSDKWrapper::TelemetryVar var(item);
    irsdk.getVarVal(var);
    Napi::Value varValue = convertTelemetryVarToObject(env, var);
    valuesObj.Set(var.header->name, varValue);
  }
  rootObj.Set("values", valuesObj);
  return rootObj;
}

Napi::Value getTelemetryDescription(const Napi::CallbackInfo& info)
{
  auto env = info.Env();
  Napi::Object obj = Napi::Object::New(env);
  std::vector<irsdk_varHeader *> headers = irsdk.getVarHeaders();

  for (const auto item : headers)
  {
    IRSDKWrapper::TelemetryVar var(item);
    irsdk.getVarVal(var);
    Napi::Object varObj = Napi::Object::New(env);
    convertVarHeaderToObject(env, var, varObj);
    obj.Set(var.header->name, varObj);
  }
  return obj;
}

void sendCmd(const Napi::CallbackInfo& info)
{
  if (!irsdk.isInitialized() || !irsdk.isConnected())
    return;

  if (info.Length() > 4 || info.Length() < 1)
  {
    std::cerr << "sendCommand: invalid arguments (1 to 4 accepted)" << std::endl;
    return;
  }

  for (int i = 0; i < info.Length(); ++i)
  {
    if (!info[i].IsNumber())
    {
      std::cerr << "sendCommand: invalid argument type, int32 needed" << std::endl;
      return;
    }
  }

  switch (info.Length())
  {
  case 1:
    broadcastCmd(
        info[0].As<Napi::Number>().Int32Value(),
        0,
        0);
    break;
  case 2:
    broadcastCmd(
        info[0].As<Napi::Number>().Int32Value(),
        info[1].As<Napi::Number>().Int32Value(),
        0);
    break;
  case 3:
    broadcastCmd(
        info[0].As<Napi::Number>().Int32Value(),
        info[1].As<Napi::Number>().Int32Value(),
        info[2].As<Napi::Number>().Int32Value());
    break;
  case 4:
    broadcastCmd(
        info[0].As<Napi::Number>().Int32Value(),
        info[1].As<Napi::Number>().Int32Value(),
        info[2].As<Napi::Number>().Int32Value(),
        info[3].As<Napi::Number>().Int32Value());
    break;
  }
}

static void cleanUp(void *arg)
{
  irsdk.shutdown();
}
} // namespace NodeIrSdk
