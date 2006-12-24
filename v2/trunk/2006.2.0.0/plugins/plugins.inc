#This is the include file to add to your plugins project

unix:OBJECTS_DIR	= ../../../build/.o/unix
win32:OBJECTS_DIR	= ../../../build/.o/win32
mac:OBJECTS_DIR	= ../../../build/.o/mac
UI_DIR	= ../../../build/.ui
MOC_DIR	= ../../../build/.moc
RCC_DIR	= ../../../build/.rcc 

MONKEY_PLUGINS_DIR	= ../../../binary/plugins

LIBS	= -lqscintilla2
win32:LIBS = -L../../../monkey -lmonkey
unix:LIBS	+= -L../../../monkey/src/3rdparty/qscintilla/Qt4

INCLUDEPATH	= ../../../monkey/src/pluginsmanager \
	../../../monkey/src/workspacemanager \
	../../../monkey/src/3rdparty/widgets/pTabWorkspace \
	 ../../../monkey/src/3rdparty/widgets/pTabToolBar \
	../../../monkey/src/monkeyeditor \
	../../../monkey/src/3rdparty/qscintilla/Qt4 \
	../../../monkey/src/3rdparty/qscintilla/Qt4/Qsci
