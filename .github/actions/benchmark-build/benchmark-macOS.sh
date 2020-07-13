#!/bin/bash
# This uses bash variable substitution in a few places
# 1. getting the cmake directory for running cpack with an absolute path (chocolatey has an unfortunately named alias)

brew install boost
mkdir build && cd build || exit
cmake -DCMAKE_BUILD_TYPE=Release -DHELICS_ZMQ_SUBPROJECT=ON -DHELICS_ENABLE_PACKAGE_BUILD=ON -DHELICS_BUILD_EXAMPLES=OFF -DHELICS_BUILD_APP_EXECUTABLES=ON -DHELICS_BUILD_APP_LIBRARY=OFF -DHELICS_BUILD_BENCHMARKS=ON -DBUILD_TESTING=OFF ..
cmake --build . --config Release
cpack_dir="$(command -v cmake)"
cpack_dir="${cpack_dir%/cmake}"
"${cpack_dir}/cpack" -G "${CPACK_GEN}" -C Release -B "$(pwd)/../artifact"
cd ../artifact || exit
rm -rf _CPack_Packages
