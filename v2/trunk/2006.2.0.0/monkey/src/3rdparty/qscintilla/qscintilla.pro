# Monkey Studio 2.0.0 QScintilla Project file
include( qscintilla.inc )
# include original project
include( $$QSCINTILLAVERSION/Qt4/qscintilla.pro )
# give defaul path
unix:OBJECTS_DIR	= ../../../../build/.o/unix
win32:OBJECTS_DIR	= ../../../../build/.o/win32
mac:OBJECTS_DIR	= ../../../../build/.o/mac
UI_DIR	= ../../../../build/.ui
MOC_DIR	= ../../../../build/.moc
RCC_DIR	= ../../../../build/.rcc
# overwrite some values
CONFIG	-= dll
CONFIG	+= staticlib
DEPENDPATH	+= $$QSCINTILLAVERSION/Qt4
INCLUDEPATH	+= $$QSCINTILLAVERSION/Qt4 $$QSCINTILLAVERSION/include $$QSCINTILLAVERSION/src
DEFINES	-= QSCINTILLA_MAKE_DLL
DESTDIR	= ../../../
