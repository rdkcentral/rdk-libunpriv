/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2020 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include<gtest/gtest.h>
#include<gmock/gmock.h>
#include <sys/capability.h>
#include "cap.h"

class CapInterface {
public:
virtual ~CapInterface() {}
virtual cap_t cap_get_proc() = 0;
virtual int cap_set_proc(cap_t cap_p) = 0;
virtual cap_t cap_get_pid(pid_t pid) = 0;
virtual int cap_clear(cap_t cap_p) = 0;
virtual int cap_clear_flag(cap_t cap_p, cap_flag_t flag) = 0;
virtual int cap_set_flag(cap_t cap_p, cap_flag_t flag, int ncap,
  const cap_value_t *capsintncap, cap_flag_value_t value) = 0;
virtual int cap_from_name(const char* name, cap_value_t *cap_p) = 0;
virtual char* cap_to_name(cap_value_t cap) = 0;
virtual char* cap_to_text(cap_t caps, ssize_t *length_p) = 0;
virtual int cap_free(void *obj_d) = 0;
virtual int cap_set_ambient(cap_value_t cap, cap_flag_value_t value) = 0;
virtual int cap_get_ambient(cap_value_t cap) = 0;
virtual uid_t getuid() = 0;
};

class CapMock : public CapInterface {
public:
virtual ~CapMock() {}
MOCK_METHOD0(cap_get_proc, cap_t());
MOCK_METHOD1(cap_set_proc, int(cap_t));
MOCK_METHOD1(cap_get_pid, cap_t(pid_t));
MOCK_METHOD1(cap_clear, int(cap_t));
MOCK_METHOD2(cap_clear_flag, int(cap_t, cap_flag_t));
MOCK_METHOD2(cap_from_name, int(const char*, cap_value_t *));
MOCK_METHOD1(cap_to_name, char*(cap_value_t));
MOCK_METHOD2(cap_to_text, char*(cap_t, ssize_t *));
MOCK_METHOD1(cap_free, int(void *));
MOCK_METHOD2(cap_set_ambient, int(cap_value_t, cap_flag_value_t));
MOCK_METHOD1(cap_get_ambient, int(cap_value_t));
        MOCK_METHOD5(cap_set_flag, int(cap_t, cap_flag_t, int, const cap_value_t *, cap_flag_value_t));
MOCK_METHOD0(getuid, uid_t());
};

