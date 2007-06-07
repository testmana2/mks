# 3drparty includes path to include in plugins/core project 

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
