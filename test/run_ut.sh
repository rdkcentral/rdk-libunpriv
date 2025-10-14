#!/bin/sh
cd ../

autoreconf --install

export top_srcdir=`pwd`

./configure --enable-gtestapp

make -C source/test

./source/test/libunpriv_gtest.bin

cd source/test
lcov --capture --directory . --output-file coverage.info
lcov --summary coverage.info


