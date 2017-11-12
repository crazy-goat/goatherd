#!/bin/bash
git submodule update --init vendor/boost/
cd vendor/boost
git submodule update --init tools/build
git submodule update --init libs/config
git submodule update --init tools/boostdep
git submodule update --init libs/process
git submodule update --init libs/thread
python tools/boostdep/depinst/depinst.py thread
./bootstrap.sh
./b2 headers
./b2