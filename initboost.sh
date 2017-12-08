#!/bin/bash
git submodule update --init vendor/boost/
git submodule update --init vendor/dir_monitor
cd vendor/boost
git submodule update --init tools/boostdep
git submodule update --init tools/build
git submodule update --init libs/config
git submodule update --init libs/process
python tools/boostdep/depinst/depinst.py process
./bootstrap.sh
./b2 --clean headers
./b2 --clean release
./b2 headers
./b2 --reconfigure --rebuild release link=static -j4
