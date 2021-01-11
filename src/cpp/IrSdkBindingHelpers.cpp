#include "IrSdkBindingHelpers.h"
#include <iostream>

using namespace Napi;
using namespace std;

Napi::Value NodeIrSdk::convertTelemetryValueToObject(Napi::Env env, IRSDKWrapper::TelemetryVar& var, const int& index)
{
  switch (var.type)
  {
  case irsdk_char:
    return Napi::String::New(env, (var.value[index]) + "\0");
  case irsdk_bool:
    return Napi::Boolean::New(env, var.boolValue[index]);
  case irsdk_int:
    if (strcmp(var.header->unit, "irsdk_SessionState") == 0)
    {
      return getStringValue(env, var.intValue[index], SESSION_STATES);
    }
    if (strcmp(var.header->unit, "irsdk_TrkLoc") == 0)
    {
      return getStringValue(env, var.intValue[index], TRACK_LOCS);
    }
    if (strcmp(var.header->unit, "irsdk_TrkSurf") == 0)
    {
      return getStringValue(env, var.intValue[index], TRACK_SURF);
    }
    if (strcmp(var.header->unit, "irsdk_PitSvStatus") == 0)
    {
      return getStringValue(env, var.intValue[index], PIT_SV_STATUS);
    }
    return Napi::Number::New(env, static_cast<int32_t>(var.intValue[index]));
  case irsdk_bitField:
    return getMaskedValues(env, var.intValue[index], var.header->unit);
  case irsdk_float:
    return Napi::Number::New(env, static_cast<double>(var.floatValue[index]));
  case irsdk_double:
    return Napi::Number::New(env, var.doubleValue[index]);
  }

  return env.Undefined();
}

Napi::Value NodeIrSdk::convertTelemetryVarToObject(Napi::Env env, IRSDKWrapper::TelemetryVar &var)
{
  if (var.header->count > 1)
  {
    Napi::Array arr = Napi::Array::New(env, var.header->count);
    for (int i = 0; i < var.header->count; ++i)
    {
      arr.Set(i, convertTelemetryValueToObject(env, var, i));
    }
    return arr;
  }
  else
  {
    return convertTelemetryValueToObject(env, var, 0);
  }
}

void NodeIrSdk::convertVarHeaderToObject(Napi::Env env, IRSDKWrapper::TelemetryVar &var, Napi::Object &obj)
{
  obj.Set("name", Napi::String::New(env, var.header->name));
  obj.Set("desc", Napi::String::New(env, var.header->desc));
  obj.Set("unit", Napi::String::New(env, var.header->unit));
  obj.Set("count", Napi::Number::New(env, var.header->count));

  switch (var.header->type)
  {
  case irsdk_char:
    obj.Set("type", Napi::String::New(env, "char"));
    break;
  case irsdk_bool:
    obj.Set("type", Napi::String::New(env, "bool"));
    break;
  case irsdk_int:
    obj.Set("type", Napi::String::New(env, "int"));
    break;
  case irsdk_bitField:
    obj.Set("type", Napi::String::New(env, "bitField"));
    break;
  case irsdk_float:
    obj.Set("type", Napi::String::New(env, "float"));
    break;
  case irsdk_double:
    obj.Set("type", Napi::String::New(env, "double"));
    break;
  default:
    obj.Set("type", Napi::String::New(env, "UNKNOWN"));
    break;
  }
}

Napi::Value NodeIrSdk::getMaskedValues(Napi::Env env, const int &val, char *unit)
{
  if (strcmp(unit, "irsdk_Flags") == 0)
  {
    return getValueArr(env, val, FLAG_MASKS);
  }
  if (strcmp(unit, "irsdk_CameraState") == 0)
  {
    return getValueArr(env, val, CAMERA_STATE_MASKS);
  }
  if (strcmp(unit, "irsdk_EngineWarnings") == 0)
  {
    return getValueArr(env, val, ENGINE_WARNINGS_MASKS);
  }
  if (strcmp(unit, "irsdk_PitSvFlags") == 0)
  {
    return getValueArr(env, val, PIT_SV_MASKS);
  }
  if (strcmp(unit, "irsdk_CarLeftRight") == 0)
  {
    return getValueArr(env, val, CAR_BESIDE);
  }
  cerr << "Missing converter for bitField: " << unit << endl;
  return Napi::Number::New(env, static_cast<int32_t>(val));
}

Napi::Array NodeIrSdk::getValueArr(Napi::Env env, const int &val, const std::vector<NodeIrSdk::MaskName> MASKS)
{
  Napi::Array arr = Napi::Array::New(env);
  int counter = 0;
  for (const auto &mask : MASKS)
  {
    if ((mask.val & val) == mask.val)
    {
      arr.Set(counter++, Napi::String::New(env, mask.name));
    }
  }
  return arr;
}

Napi::Value NodeIrSdk::getStringValue(Napi::Env env, const int &val, const std::vector<NodeIrSdk::MaskName> &map)
{
  for (const auto &mask : map)
  {
    if (mask.val == val)
    {
      return Napi::String::New(env, mask.name);
    }
  }
  return env.Undefined();
}

NodeIrSdk::MaskName::MaskName(int val, const char *name) : val(val), name(name)
{
}
