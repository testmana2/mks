LIBS *=  -l$$libName(CppEditor) -l$$libName(CppTools ) \
	-l$$libName(Debugger) -l$$libName(ProjectExplorer) \
	-l$$libName(TextEditor) -l$$libName(Find) \
	-l$$libName(QuickOpen) -l$$libName(Core)

PRE_TARGETDEPS *= $$PWD/debugger $$PWD/texteditor $$PWD/find $$PWD/projectexplorer $$PWD/coreplugin

INCLUDEPATH *=  $$PWD/debugger

#hack for include to main.cpp ui_findwidget.h
INCLUDEPATH *=  $$PWD/find/.uic