LIBS *=  -lDebugger -lTextEditor -lFind -lProjectExplorer -lCore 

PRE_TARGETDEPS *= $$PWD/debugger $$PWD/texteditor $$PWD/find $$PWD/projectexplorer $$PWD/coreplugin

INCLUDEPATH *=  $$PWD/debugger
