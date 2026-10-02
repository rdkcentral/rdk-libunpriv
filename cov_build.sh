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

# Coverity capture of the build
echo "Initializing Coverity build capture..."
if command -v cov-build &> /dev/null; then
    cov-build --dir cov-capture make
    cov-analyze --dir cov-capture --strip-path "${GITHUB_WORKSPACE}"
    cov-commit-defects \
        --dir cov-capture \
        --url "${COVERITY_URL}" \
        --stream "${COVERITY_STREAM}" \
        --user "${COVERITY_USER}" \
        --password "${COVERITY_PASSWORD}"
fi

echo "Installing..."
make install

echo "======================================================================================"
echo "Build completed successfully"
exit 0

