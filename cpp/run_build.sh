#!/bin/bash

START_DIR=`pwd`
BUILD_DIR="${START_DIR}/build"
SRC_DIR="${START_DIR}/src"

#make build dir if not there
if [ ! -d $BUILD_DIR ]; then
      mkdir -p $BUILD_DIR;
fi

#clean and build
cd $BUILD_DIR;
#rm -rf *;
cmake $SRC_DIR;
make;
