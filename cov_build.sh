#!/bin/bash
set -x
set -e
##############################
export PATH=/usr/local/bin:$PATH

GITHUB_WORKSPACE="${PWD}"
ls -la ${GITHUB_WORKSPACE}

############################
# Build rdk-libunpriv
echo "building rdk-libunpriv"

cd ${GITHUB_WORKSPACE}

# Configure and build using autotools
echo "Running autoreconf..."
autoreconf -i

echo "Running configure..."
./configure \
    --prefix="${GITHUB_WORKSPACE}/install/usr" \
    CFLAGS="-fvisibility=default" \
    CXXFLAGS="-fvisibility=default"

echo "Building with make..."
make

echo "Installing..."
make install

echo "======================================================================================"
echo "Build completed successfully"
exit 0

