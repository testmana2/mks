# Monkey Studio 2.0.0 QScintilla Project file
include( qscintilla_check.pri )

# include path
isEqual( SYSTEM_QSCINTILLA, 1 ) {
    # include path
    INCLUDEPATH *= $$[QT_INSTALL_HEADERS]/Qsci
} else {
    # define where is the official qscintilla folder
    QSCINTILLAVERSION   = QScintilla-gpl-snapshot

    # include path
    INCLUDEPATH *= $${PWD}/$${QSCINTILLAVERSION}/include $${PWD}/$${QSCINTILLAVERSION}/src $${PWD}/$${QSCINTILLAVERSION}/lexers $${PWD}/$${QSCINTILLAVERSION}/lexlib $${PWD}/$${QSCINTILLAVERSION}/Qt4Qt5 $${PWD}/$${QSCINTILLAVERSION}/Qt4Qt5/Qsci
    #INCLUDEPATH    *= $$getFolders( $${PWD}/$${QSCINTILLAVERSION}, designer-Qt3 designer-Qt4 example-Qt3 example-Qt4Qt5 Qt3 doc Python python api )

    # dependency
    PRE_TARGETDEPS  *= $${PWD}

    # library integration
    LIBS    *= -L$${PACKAGE_BUILD_PATH}/$${Q_TARGET_ARCH}/$$buildMode()
}

QSCINTILLA_TARGET   = qscintilla2
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets printsupport

contains( TEMPLATE, .*app ) {
    CONFIG(debug, debug|release) {
        #Debug
        isEqual( SYSTEM_QSCINTILLA, 1 ) {
            LIBS    *= -l$${QSCINTILLA_TARGET}
        } else {
            unix:LIBS   *= -l$${QSCINTILLA_TARGET}_debug
            else:LIBS   *= -l$${QSCINTILLA_TARGET}d
        }
    } else {
        #Release
        LIBS    *= -l$${QSCINTILLA_TARGET}
    }
}
