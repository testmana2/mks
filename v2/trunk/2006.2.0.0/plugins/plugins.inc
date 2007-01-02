#This is the include file to add to your plugins project

include( ../../../monkey/src/3rdparty/qscintilla/qscintilla.inc )

unix:OBJECTS_DIR	= ../../../build/.o/unix
win32:OBJECTS_DIR	= ../../../build/.o/win32
mac:OBJECTS_DIR	= ../../../build/.o/mac
UI_DIR	= ../../../build/.ui
MOC_DIR	= ../../../build/.moc
RCC_DIR	= ../../../build/.rcc 

MONKEY_PLUGINS_DIR	= ../../../binary/plugins

LIBS	= -L../../../build/.lib -lqscintilla2
win32-g++:LIBS += -lmonkey

INCLUDEPATH	= ../../../monkey/src/pluginsmanager \
	../../../monkey/src/workspacemanager \
	../../../monkey/src/3rdparty/widgets/pTabWorkspace \
	 ../../../monkey/src/3rdparty/widgets/pTabToolBar \
	../../../monkey/src/monkeyeditor \
	../../../monkey/src/3rdparty/qscintilla/$$QSCINTILLAVERSION/Qt4 \
	../../../monkey/src/3rdparty/qscintilla/$$QSCINTILLAVERSION/Qt4/Qsci \
	../../../monkey/src/projectsmanager \
	../../../monkey/src/projectsmanager/ui
