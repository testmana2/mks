TEMPLATE = lib
TARGET = Debugger

# CONFIG += single

# DEFINES += QT_NO_CAST_FROM_ASCII
DEFINES += QT_NO_CAST_TO_ASCII
DEFINES += GDBDEBUGGERLEAN
DEFINES += Q_SLOT=

QT += gui network script

HEADERS += attachexternaldialog.h \
    attachremotedialog.h \
    breakhandler.h \
    breakwindow.h \
    debuggerconstants.h \
    debuggermanager.h \
    debuggeroutputwindow.h \
    debuggerrunner.h \
    disassemblerhandler.h \
    disassemblerwindow.h \
    gdbengine.h \
    gdbmi.h \
    idebuggerengine.h \
    moduleshandler.h \
    moduleswindow.h \
    outputcollector.h \
    procinterrupt.h \
    registerhandler.h \
    registerwindow.h \
    scriptengine.h \
    stackhandler.h \
    stackwindow.h \
    sourcefileswindow.h \
    startexternaldialog.h \
    threadswindow.h \
    watchhandler.h \
    watchwindow.h

SOURCES += attachexternaldialog.cpp \
    attachremotedialog.cpp \
    breakhandler.cpp \
    breakwindow.cpp \
    breakwindow.h \
    debuggermanager.cpp \
    debuggeroutputwindow.cpp \
    debuggerrunner.cpp \
    disassemblerhandler.cpp \
    disassemblerwindow.cpp \
    gdbengine.cpp \
    gdbmi.cpp \
    moduleshandler.cpp \
    moduleswindow.cpp \
    outputcollector.cpp \
    procinterrupt.cpp \
    registerhandler.cpp \
    registerwindow.cpp \
    scriptengine.cpp \
    stackhandler.cpp \
    stackwindow.cpp \
    sourcefileswindow.cpp \
    startexternaldialog.cpp \
    threadswindow.cpp \
    watchhandler.cpp \
    watchwindow.cpp

FORMS += attachexternaldialog.ui \
    attachremotedialog.ui \
    breakbyfunction.ui \
    breakcondition.ui \
    gdboptionpage.ui \
    startexternaldialog.ui \

RESOURCES += debugger.qrc

false {
SOURCES += $$PWD/modeltest.cpp
HEADERS += $$PWD/modeltest.h
DEFINES += USE_MODEL_TEST=1
}
