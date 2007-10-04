# Monkey Studio 2.0.0 QScintilla Project file

# define where is the official qscintilla folder
QSCINTILLAVERSION	= QScintilla-gpl-2-snapshot-20070923

# get relative path
QPRO_PWD   = $$PWD

# include path
INCLUDEPATH	*= $${QPRO_PWD}/$$QSCINTILLAVERSION/Qt4
INCLUDEPATH	*= $${QPRO_PWD}/$$QSCINTILLAVERSION/Qt4/Qsci
INCLUDEPATH	*= $${QPRO_PWD}/$$QSCINTILLAVERSION/include
INCLUDEPATH	*= $${QPRO_PWD}/$$QSCINTILLAVERSION/src

#depend path
DEPENDPATH	*= $${QPRO_PWD}/$$QSCINTILLAVERSION/Qt4
DEPENDPATH	*= $${QPRO_PWD}/$$QSCINTILLAVERSION/Qt4/Qsci
DEPENDPATH	*= $${QPRO_PWD}/$$QSCINTILLAVERSION/include
DEPENDPATH	*= $${QPRO_PWD}/$$QSCINTILLAVERSION/src
