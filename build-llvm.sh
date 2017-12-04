#!/usr/bin/env bash
cmake -DCMAKE_BUILD_TYPE=Release .
make clean
make VERBOSE=1 -j4