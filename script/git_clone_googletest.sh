#!/bin/bash
# we have not git on docker containers (only compilers)
set -e

SCRIPT_DIR="$(dirname "$0")"
REPO="https://github.com/google/googletest.git"
TAG="v1.17.0"

if [ ! -d "${SCRIPT_DIR}/googletest" ]; then
    git clone --branch "${TAG}" --depth 1 --no-tags "${REPO}" "${SCRIPT_DIR}/googletest"
else
    pushd "${SCRIPT_DIR}/googletest"
    git fetch origin tag "${TAG}" --depth 1 --no-tags
    git reset --hard FETCH_HEAD
    popd
fi
