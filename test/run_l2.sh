#!/bin/sh
####################################################################################
# If not stated otherwise in this file or this component's Licenses.txt file the
# following copyright and licenses apply:
#
# Copyright 2024 RDK Management
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
####################################################################################
echo "===== rdk-libunpriv L2 test ====="

export top_srcdir=$(pwd | sed 's#/test##')
RESULT_DIR="/tmp/libunpriv_l2_report"
mkdir -p "$RESULT_DIR"

echo "top_srcdir = $top_srcdir"
echo "RESULT_DIR = $RESULT_DIR"



###############################################################################
# Step 1: Build library
###############################################################################
cd "$top_srcdir"
autoreconf --install
./configure
make clean
make

###############################################################################
# Step 2: Build L2 helper
###############################################################################
cd test/functional-tests/tests
make clean
make

###############################################################################
# Step 3: Run pytest
###############################################################################
cd ..
pytest -v --json-report --json-report-file "$RESULT_DIR/libunpriv_l2.json" tests/

echo "===== L2 execution completed ====="
echo "Results at: $RESULT_DIR/libunpriv_l2.json"
