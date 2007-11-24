#! /bin/sh
qmake -spec macx-g++ "CONFIG += debug_and_release"
make -f Makefile distclean
qmake -spec macx-g++ "CONFIG += debug_and_release"
make -f Makefile release