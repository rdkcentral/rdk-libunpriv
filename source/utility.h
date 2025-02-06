/*
* Copyright 2021 Comcast Cable Communications Management, LLC
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
*
* SPDX-License-Identifier: Apache-2.0
*/
#ifndef __UTILITY_H
#define __UTILITY_H
#include "cap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TOKEN_DELIMITER ","

#ifdef _RDK_BROADBAND_PRIV_CAPS_
        #ifdef _COSA_INTEL_XB3_ARM_
                #define LOG_FILE "/rdklogs/logs/CapDebug.txt"
        #elif _COSA_INTEL_USG_ATOM_
                #define LOG_FILE "/rdklogs/logs/CapDebug_atom.txt"
        #else
                #define LOG_FILE "/rdklogs/logs/CapDebug.txt"
        #endif
#elif  defined (_RDK_VIDEO_PRIV_CAPS_)
        #define LOG_FILE "/opt/logs/CapDebug.txt"
#else
        #define LOG_FILE "/rdklogs/logs/CapDebug.txt"
#endif

/* Read the default,allowed and drop capabilities from CAP_FILE */
void get_capabilities(const char *processname, cap_user *);

/* Logger for libunpriv */
void log_cap(const char * format, ...);

#ifdef __cplusplus
}
#endif
#endif
