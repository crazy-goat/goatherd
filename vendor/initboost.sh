#!/bin/bash
if [ ! -d "boost" ]; then
    if [ ! -f "boost.tar.gz" ]; then
        wget https://dl.bintray.com/boostorg/release/1.65.1/source/boost_1_65_1.tar.gz -O boost.tar.gz
    fi
    tar xvf boost.tar.gz
    mv boost_1_65_1 boost
fi
cd boost
if [ ! -f "b2" ]; then
./bootstrap.sh
fi
./b2 headers
./b2 release link=static -j4