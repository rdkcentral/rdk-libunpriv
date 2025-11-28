#!/bin/sh
cd ../

ENABLE_COV=true

if [ "x$1" = "x--enable-cov" ]; then
    echo "Enabling coverage options"
    export CXXFLAGS="-g -O0 -fprofile-arcs -ftest-coverage"
    export CFLAGS="-g -O0 -fprofile-arcs -ftest-coverage"
    export LDFLAGS="-lgcov --coverage"
    ENABLE_COV=true
fi

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
echo "********************"

if [ "$ENABLE_COV" = true ]; then
    echo "Generating coverage report"
    lcov --capture --directory . --output-file coverage.info
    lcov --remove coverage.info '/usr/*' '*gmocks*' '*test*' --output-file coverage.info
    lcov --list coverage.info
fi
