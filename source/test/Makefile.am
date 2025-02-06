#
# If not stated otherwise in this file or this component's LICENSE file the
# following copyright and licenses apply:
#
# Copyright 2020 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
AM_CXXFLAGS = -std=c++14 -DGTEST_ENABLE -g --coverage

AM_CFLAGS = -DGTEST_ENABLE

AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = libunpriv_gtest.bin

libunpriv_gtest_bin_CPPFLAGS = -I$(PKG_CONFIG_SYSROOT_DIR)/googletest/include/gtest -I$(PKG_CONFIG_SYSROOT_DIR)/googletest/include/ -I$(PKG_CONFIG_SYSROOT_DIR)/googletest/googletest/include -I$(PKG_CONFIG_SYSROOT_DIR)/googletest/googlemock/include/ -I${top_srcdir}/source -I${top_srcdir}/source/include

libunpriv_gtest_bin_SOURCES = gtest_main.cpp  libunpriv_test.cpp ../cap.c ../utility.cpp  mocks/CapMock.cpp

libunpriv_gtest_bin_LDFLAGS = -L/$(PKG_CONFIG_SYSROOT_DIR)/googletest/lib  -lgmock -lgtest -lpthread -lgcov -lgtest_main -ljsoncpp

