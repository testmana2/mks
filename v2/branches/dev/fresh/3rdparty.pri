# 3drparty includes path to include in plugins/core project 

# include path
INCLUDEPATH	= .

# for normal build, comment for build dll/lib
DEFINES	+= MONKEY_CORE_BUILD

# QScintilla
include( qscintilla/qscintilla.pri )
INCLUDEPATH	+= $$QSCINTILLAVERSION/Qt4 \
	$$QSCINTILLAVERSION/include \
	$$QSCINTILLAVERSION/src

# Coverage meter
INCLUDEPATH	+= coveragemeter \
	coveragemeter/ui

# Objects
INCLUDEPATH	+= objects

# Widgets
INCLUDEPATH	+= widgets

# dependpath as includepath
DEPENDPATH	= $$INCLUDEPATH
