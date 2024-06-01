#!/bin/bash

PROJECT="co-cpp19"
IMAGE="arbmind/cmake-gcc:latest"
BUILD_VOLUME="${PROJECT}-cmake-build"

cd "$(dirname "$0")/.."
docker run -it --rm --mount "src=$(pwd),target=/project,type=bind" -w /project --mount "src=${BUILD_VOLUME},target=/project/build,type=volume" "${IMAGE}" "$@"
