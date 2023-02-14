#!/bin/sh

set -e

cmake --build build -j8
3rd/testa/runtests.py --lang 3rd/testa/lang.json "$@" build/test/test_exe
