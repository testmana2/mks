#! /bin/sh
cd patchs-for-3rdparty
./patch.sh
cd ..
qmake-qt4 "CONFIG += debug_and_release console"
make -f Makefile debug
