#!/bin/bash

PROJECT="co-cpp19"
IMAGE="conanio/gcc12-ubuntu18.04:latest"
BUILD_VOLUME="${PROJECT}-conan-build"

cd "$(dirname "$0")/.."
docker run -it --rm --mount "src=$(pwd),target=/project,type=bind" -w /project --mount "src=${BUILD_VOLUME},target=/project/build,type=volume" "${IMAGE}" "$@"
