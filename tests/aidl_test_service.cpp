/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <binder/IInterface.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/Looper.h>
#include <utils/StrongPointer.h>

#include "android/aidl/tests/BnTestService.h"
#include "android/aidl/tests/ITestService.h"

#include "android/aidl/tests/BnNamedCallback.h"
#include "android/aidl/tests/INamedCallback.h"

// Used implicitly.
#undef LOG_TAG
#define LOG_TAG "aidl_native_service"

// libutils:
using android::Looper;
using android::LooperCallback;
using android::OK;
using android::sp;
using android::status_t;
using android::String16;

// libbinder:
using android::BnInterface;
using android::defaultServiceManager;
using android::IInterface;
using android::IPCThreadState;
using android::Parcel;
using android::ProcessState;

// Generated code:
using android::aidl::tests::BnTestService;
using android::aidl::tests::BnNamedCallback;
using android::aidl::tests::INamedCallback;

// Standard library
using std::map;
using std::vector;

namespace android {
namespace generated {
namespace {

class BinderCallback : public LooperCallback {
 public:
  BinderCallback() {}
  ~BinderCallback() override {}

  int handleEvent(int /* fd */, int /* events */, void* /* data */ ) override {
    IPCThreadState::self()->handlePolledCommands();
    return 1;  // Continue receiving callbacks.
  }
};

class NamedCallback : public BnNamedCallback {
 public:
  NamedCallback(String16 name) : name_(name) {}

  status_t GetName(String16* ret) {
    *ret = name_;
    return OK;
  }

 private:
  String16 name_;
};

class NativeService : public BnTestService {
 public:
  NativeService() {}
  ~NativeService() override {}

  int Run() {
    sp<Looper> looper(Looper::prepare(0 /* opts */));

    int binder_fd = -1;
    ProcessState::self()->setThreadPoolMaxThreadCount(0);
    IPCThreadState::self()->disableBackgroundScheduling(true);
    IPCThreadState::self()->setupPolling(&binder_fd);
    ALOGI("Got binder FD %d", binder_fd);
    if (binder_fd < 0) return -1;

    sp<BinderCallback> cb(new BinderCallback);
    if (looper->addFd(binder_fd, Looper::POLL_CALLBACK, Looper::EVENT_INPUT, cb,
                      nullptr) != 1) {
      ALOGE("Failed to add binder FD to Looper");
      return -1;
    }

    defaultServiceManager()->addService(getInterfaceDescriptor(), this);

    ALOGI("Entering loop");
    while (true) {
      const int result = looper->pollAll(-1 /* timeoutMillis */);
      ALOGI("Looper returned %d", result);
    }
    return 0;
  }

  void LogRepeatedStringToken(const String16& token) {
    ALOGI("Repeating '%s' of length=%zu", android::String8(token).string(),
          token.size());
  }

  template<typename T>
  void LogRepeatedToken(const T& token) {
    std::ostringstream token_str;
    token_str << token;
    ALOGI("Repeating token %s", token_str.str().c_str());
  }

  status_t RepeatBoolean(bool token, bool* _aidl_return) override {
    LogRepeatedToken(token ? 1 : 0);
    *_aidl_return = token;
    return OK;
  }
  status_t RepeatByte(int8_t token, int8_t* _aidl_return) override {
    LogRepeatedToken(token);
    *_aidl_return = token;
    return OK;
  }
  status_t RepeatChar(char16_t token, char16_t* _aidl_return) override {
    LogRepeatedStringToken(String16(&token, 1));
    *_aidl_return = token;
    return OK;
  }
  status_t RepeatInt(int32_t token, int32_t* _aidl_return) override {
    LogRepeatedToken(token);
    *_aidl_return = token;
    return OK;
  }
  status_t RepeatLong(int64_t token, int64_t* _aidl_return) override {
    LogRepeatedToken(token);
    *_aidl_return = token;
    return OK;
  }
  status_t RepeatFloat(float token, float* _aidl_return) override {
    LogRepeatedToken(token);
    *_aidl_return = token;
    return OK;
  }
  status_t RepeatDouble(double token, double* _aidl_return) override {
    LogRepeatedToken(token);
    *_aidl_return = token;
    return OK;
  }
  status_t RepeatString(
      const String16& token, String16* _aidl_return) override {
    LogRepeatedStringToken(token);
    *_aidl_return = token;
    return OK;
  }

  template<typename T>
  status_t ReverseArray(const vector<T>& input,
                        vector<T>* repeated,
                        vector<T>* _aidl_return) {
    ALOGI("Reversing array of length %zu", input.size());
    *repeated = input;
    *_aidl_return = input;
    std::reverse(_aidl_return->begin(), _aidl_return->end());
    return OK;
  }

  status_t ReverseBoolean(const vector<bool>& input,
                          vector<bool>* repeated,
                          vector<bool>* _aidl_return) override {
    return ReverseArray(input, repeated, _aidl_return);
  }
  status_t ReverseByte(const vector<int8_t>& input,
                       vector<int8_t>* repeated,
                       vector<int8_t>* _aidl_return) override {
    return ReverseArray(input, repeated, _aidl_return);
  }
  status_t ReverseChar(const vector<char16_t>& input,
                       vector<char16_t>* repeated,
                       vector<char16_t>* _aidl_return) override {
    return ReverseArray(input, repeated, _aidl_return);
  }
  status_t ReverseInt(const vector<int32_t>& input,
                      vector<int32_t>* repeated,
                      vector<int32_t>* _aidl_return) override {
    return ReverseArray(input, repeated, _aidl_return);
  }
  status_t ReverseLong(const vector<int64_t>& input,
                       vector<int64_t>* repeated,
                       vector<int64_t>* _aidl_return) override {
    return ReverseArray(input, repeated, _aidl_return);
  }
  status_t ReverseFloat(const vector<float>& input,
                        vector<float>* repeated,
                        vector<float>* _aidl_return) override {
    return ReverseArray(input, repeated, _aidl_return);
  }
  status_t ReverseDouble(const vector<double>& input,
                         vector<double>* repeated,
                         vector<double>* _aidl_return) override {
    return ReverseArray(input, repeated, _aidl_return);
  }
  status_t ReverseString(const vector<String16>& input,
                         vector<String16>* repeated,
                         vector<String16>* _aidl_return) override {
    return ReverseArray(input, repeated, _aidl_return);
  }

  status_t GetOtherTestService(const String16& name,
                               sp<INamedCallback>* returned_service) override {
    if (service_map_.find(name) == service_map_.end()) {
      sp<INamedCallback> new_item(new NamedCallback(name));
      service_map_[name] = new_item;
    }

    *returned_service = service_map_[name];
    return OK;
  }

  status_t VerifyName(const sp<INamedCallback>& service, const String16& name,
                      bool* returned_value) override {
    String16 foundName;
    status_t err = service->GetName(&foundName);

    if (err == OK) {
      *returned_value = foundName == name;
    }

    return err;
  }

 private:
  map<String16, sp<INamedCallback>> service_map_;
};

}  // namespace
}  // namespace generated
}  // namespace android

int main(int /* argc */, char* /* argv */ []) {
  android::generated::NativeService service;
  return service.Run();
}