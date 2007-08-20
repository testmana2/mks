qmake
mingw32-make -f Makefile.Debug distclean
qmake "CONFIG*=debug"
mingw32-make -f Makefile.Debug
pause
