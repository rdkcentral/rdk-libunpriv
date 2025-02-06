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
#include "test/mocks/CapMock.h"

extern CapMock *g_CapMock;

extern "C" cap_t cap_get_proc()
{
if(!g_CapMock)
{
return nullptr;
}
return g_CapMock->cap_get_proc();
}
extern "C" int cap_set_proc(cap_t cap_p)
{
if(!g_CapMock)
{
return -1;
}
return g_CapMock->cap_set_proc(cap_p);
}
extern "C" cap_t cap_get_pid (pid_t pid)
{
        if(!g_CapMock)
        {
                return nullptr;
        }
        return g_CapMock->cap_get_pid(pid);
}
extern "C" int cap_clear (cap_t cap_p)
{
        if(!g_CapMock)
        {
                return -1;
        }
        return g_CapMock->cap_clear(cap_p);
}
extern "C" int cap_clear_flag (cap_t cap_p, cap_flag_t flag)
{
        if(!g_CapMock)
        {
                return -1;
        }
        return g_CapMock->cap_clear_flag(cap_p, flag);
}
extern "C" int cap_set_flag(cap_t cap_p, cap_flag_t flag, int ncap,
  const cap_value_t *capsintncap, cap_flag_value_t value)
{
        if(!g_CapMock)
        {
                return -1;
        }
        return g_CapMock->cap_set_flag(cap_p, flag, ncap, capsintncap, value);
}
extern "C" int cap_from_name (const char* name, cap_value_t *cap_p)
{
        if(!g_CapMock)
        {
                return -1;
        }
        return g_CapMock->cap_from_name(name, cap_p);
}
extern "C" char* cap_to_name (cap_value_t cap)
{
        if(!g_CapMock)
        {
                return nullptr;
        }
        return g_CapMock->cap_to_name(cap);
}
extern "C" char* cap_to_text (cap_t caps, ssize_t *length_p)
{
        if(!g_CapMock)
        {
                return nullptr;
        }
        return g_CapMock->cap_to_text(caps,length_p);
}
extern "C" int cap_free (void *obj_d)
{
        if(!g_CapMock)
        {
                return -1;
        }
        return g_CapMock->cap_free(obj_d);
}
extern "C" int cap_set_ambient(cap_value_t cap, cap_flag_value_t value)
{
        if(!g_CapMock)
        {
                return -1;
        }
        return g_CapMock->cap_set_ambient(cap, value);
}
extern "C" int cap_get_ambient(cap_value_t cap)
{
        if(!g_CapMock)
        {
                return -1;
        }
        return g_CapMock->cap_get_ambient(cap);
}
extern "C" uid_t getuid()
{
        if(!g_CapMock)
        {
                return -1;
        }
        return g_CapMock->getuid();
}

