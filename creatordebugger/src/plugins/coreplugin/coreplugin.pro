TEMPLATE = lib
TARGET = Core
DEFINES += CORE_LIBRARY
QT += xml \
    script \
    svg

include(../../qworkbenchplugin.pri)
include(../../libs/utils/utils.pri)
include(../../shared/scriptwrapper/scriptwrapper.pri)
include(coreplugin_dependencies.pri)
INCLUDEPATH += . \
    dialogs \
    actionmanager \
    editormanager
DEPENDPATH += dialogs \
    actionmanager \
    editormanager
SOURCES += \
    basemode.cpp \
    uniqueidmanager.cpp \
    modemanager.cpp \
    findplaceholder.cpp \
    icore.cpp
HEADERS += \
    uniqueidmanager.h \
    outputpane.h \
    editormanager/ieditor.h \
    actionmanager/actioncontainer.h \
    actionmanager/actionmanager.h \
    actionmanager/command.h \
    dialogs/ioptionspage.h \
    icontext.h \
    icore.h \
    ifile.h \
    imode.h \
    coreconstants.h \
    basemode.h \
    modemanager.h \
    plugindialog.h \
    navigationwidget.h \
    findplaceholder.h \
    rightpane.h \
    editormanager.h
