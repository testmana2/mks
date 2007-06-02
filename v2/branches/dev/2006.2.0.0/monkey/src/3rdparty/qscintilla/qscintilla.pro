# Monkey Studio 2.0.0 QScintilla Project file
include( qscintilla.inc )
# include original project
include( $$QSCINTILLAVERSION/Qt4/qscintilla.pro )
# give defaul path
unix:OBJECTS_DIR	= ../../../../build/.o/unix/qscintilla
win32:OBJECTS_DIR	= ../../../../build/.o/win32/qscintilla
mac:OBJECTS_DIR	= ../../../../build/.o/mac/qscintilla
UI_DIR	= ../../../../build/.ui
MOC_DIR	= ../../../../build/.moc/qscintilla
RCC_DIR	= ../../../../build/.rcc/qscintilla
# overwrite some values
CONFIG	-= dll
CONFIG	+= staticlib
DEPENDPATH	+= $$QSCINTILLAVERSION/Qt4
INCLUDEPATH	+= $$QSCINTILLAVERSION/Qt4 $$QSCINTILLAVERSION/include $$QSCINTILLAVERSION/src
DEFINES	-= QSCINTILLA_MAKE_DLL
DESTDIR	= ../../../../build/.lib


CoverageMeter {
# Configuration for code coverage using 
# CoverageMeter (http://www.coveragemeter.com)
  QMAKE_CXX=cs$$QMAKE_CXX
  QMAKE_LINK=cs$$QMAKE_LINK
  QMAKE_CC=cs$$QMAKE_CC
}

