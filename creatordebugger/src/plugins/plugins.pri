LIBS *= -Wl,--whole-archive  -lCore -lDebugger -lFind -lProjectExplorer -lTextEditor -Wl,--no-whole-archive

INCLUDEPATH *=  $$PWD/debugger
