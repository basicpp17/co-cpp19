#!/bin/bash
git clone -b release-1.10.0 --depth 1 --no-tags https://github.com/google/googletest.git "$(dirname "$0")/googletest"
