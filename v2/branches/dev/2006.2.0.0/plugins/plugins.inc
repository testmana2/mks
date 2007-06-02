#This is the include file to add to your plugins project

include( ../../../monkey/src/3rdparty/qscintilla/qscintilla.inc )

unix:OBJECTS_DIR	= ../../../build/.o/unix/plugins
win32:OBJECTS_DIR	= ../../../build/.o/win32/plugins
mac:OBJECTS_DIR	= ../../../build/.o/mac/plugins
UI_DIR	= ../../../build/.ui
MOC_DIR	= ../../../build/.moc/plugins
RCC_DIR	= ../../../build/.rcc/plugins

MONKEY_PLUGINS_DIR	= ../../../bin/plugins
#DEFINES	= __COVERAGESCANNER__
LIBS	= -L../../../build/.lib -lqscintilla2
win32:LIBS += -lmonkey
win32:QMAKE_LIBDIR	+=../../../bin

contains( DEFINES, __COVERAGESCANNER__ ) {
	message( "Coverage Meter Activated ( http://www.coveragemeter.com )." )
	QMAKE_CXX = cs$$QMAKE_CXX
	QMAKE_LINK = cs$$QMAKE_LINK
	QMAKE_CC = cs$$QMAKE_CC
}

INCLUDEPATH	= . \
	src \
	src/ui \
	../../bridges/Console/src \
	../../../monkey/src/pluginsmanager \
	../../../monkey/src/workspacemanager \
	../../../monkey/src/maininterface \
	../../../monkey/src/3rdparty/objects/pSettings \
	../../../monkey/src/3rdparty/widgets/pMenuBar \
	../../../monkey/src/3rdparty/widgets/pTabWorkspace \
	../../../monkey/src/3rdparty/widgets/pTabToolBar \
	../../../monkey/src/monkeyeditor \
	../../../monkey/src/3rdparty/qscintilla/$$QSCINTILLAVERSION/Qt4 \
	../../../monkey/src/3rdparty/qscintilla/$$QSCINTILLAVERSION/Qt4/Qsci \
	../../../monkey/src/projectsmanager \
	../../../monkey/src/projectsmanager/ui \
	../../../monkey/src
