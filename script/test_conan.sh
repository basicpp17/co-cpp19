#!/bin/bash
set -e # stop on first error

# default arguments:
#   script/test_conan.sh array19
ConanCase=${1:-array19}
shift

BUILD_PATH="build/conan-${ConanCase}"
SOURCE_PATH="conan/tests/${ConanCase}"
ConfigPreset="conan-release" # "conan-default" for multi config generators
BuildPreset="conan-release"
TestPreset="conan-release"

cd "$(dirname "${BASH_SOURCE[0]}")/.."
BASE_DIR="$(pwd)"

mkdir -p ${BUILD_PATH}
cd ${BUILD_PATH}
conan install "${BASE_DIR}/${SOURCE_PATH}" --output-folder=. "$@"
cmake "${BASE_DIR}/${SOURCE_PATH}" --preset "${ConfigPreset}"
cmake --build --preset "${BuildPreset}"
ctest --preset "${TestPreset}"
