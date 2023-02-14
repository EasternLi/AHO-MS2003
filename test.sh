#!/bin/sh

cd build
make
cd ..
3rd/testa/runtests.py --lang 3rd/testa/lang.json build/test/randomTest
