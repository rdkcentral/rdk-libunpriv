#!/bin/sh

# Copyright 2023 Comcast Cable Communications Management, LLC
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
# SPDX-License-Identifier: Apache-2.0
#

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
TOP_DIR=$(cd "$SCRIPT_DIR/.." && pwd)
cd "$TOP_DIR" || exit 1

echo "Enabling coverage options"
export CXXFLAGS="-g -O0 -fprofile-arcs -ftest-coverage"
export CFLAGS="-g -O0 -fprofile-arcs -ftest-coverage"
export LDFLAGS="-lgcov --coverage"

autoreconf --install

./configure --enable-gtestapp

make clean
find . -name "*.gcda" -delete
find . -name "*.gcno" -delete
make -C source/test

echo "**** Compiling libunpriv_gtest ****"

./source/test/libunpriv_gtest.bin

if [ $? -ne 0 ]; then
    echo "Unit tests failed"
    exit 1
fi

echo "Generating coverage report"
lcov --capture --directory .. --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*gmocks*' '*test*' --output-file coverage.info
lcov --list coverage.info
genhtml coverage.info --output-directory out 

