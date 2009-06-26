LIBS *=  -lCppEditor -lCppTools -lDebugger -lProjectExplorer -lTextEditor -lFind  -lQuickOpen -lCore 

PRE_TARGETDEPS *= $$PWD/debugger $$PWD/texteditor $$PWD/find $$PWD/projectexplorer $$PWD/coreplugin

INCLUDEPATH *=  $$PWD/debugger

#hack for include to main.cpp ui_findwidget.h
INCLUDEPATH *=  $$PWD/find/.uic