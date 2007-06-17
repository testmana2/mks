# Monkey Studio 2.0.0 QScintilla Project file
include( qscintilla.pri )

# include original project
include( $$QSCINTILLAVERSION/Qt4/qscintilla.pro )

# give defaul path
BUILD_PATH	= ../build

unix:OBJECTS_DIR	= $$BUILD_PATH/.o/unix/qscintilla
win32:OBJECTS_DIR	= $$BUILD_PATH/.o/win32/qscintilla
mac:OBJECTS_DIR	= $$BUILD_PATH/.o/mac/qscintilla
UI_DIR	= $$BUILD_PATH/.ui
MOC_DIR	= $$BUILD_PATH/.moc/qscintilla
RCC_DIR	= $$BUILD_PATH/.rcc/qscintilla

# overwrite some values
CONFIG	-= dll
CONFIG	+= staticlib
DEPENDPATH	+= $$QSCINTILLAVERSION/Qt4
INCLUDEPATH	+= $$QSCINTILLAVERSION/Qt4 $$QSCINTILLAVERSION/include $$QSCINTILLAVERSION/src
DEFINES	-= QSCINTILLA_MAKE_DLL
DESTDIR	= $$BUILD_PATH/.lib
