#!/bin/sh

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

if [ $fail -ne 0 ]; then
    echo "Some unit tests failed."
    exit 1
else
    echo "All unit tests passed."
fi
