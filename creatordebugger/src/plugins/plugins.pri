LIBS *=  -lDebugger -lProjectExplorer -lTextEditor -lFind  -lQuickOpen -lCore 

PRE_TARGETDEPS *= $$PWD/debugger $$PWD/texteditor $$PWD/find $$PWD/projectexplorer $$PWD/coreplugin

INCLUDEPATH *=  $$PWD/debugger
