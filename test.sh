#!/bin/sh

set -e

build=cmake-build-release
if [ ! -d $build ]; then
	cmake -B $build -D CMAKE_BUILD_TYPE=Release
fi

cmake --build $build -j8
3rd/testa/runtests.py --lang 3rd/testa/lang.json "$@" $build/test/test_exe
