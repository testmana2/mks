TEMPLATE = lib
CONFIG *= staticlib

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
    mainwindow.cpp \
    coreimpl.cpp \
    basemode.cpp \
    uniqueidmanager.cpp \
    modemanager.cpp \
    findplaceholder.cpp \
    rightpane.cpp \
    outputpane.cpp \
    navigationwidget.cpp \
    mimedatabase.cpp \
    editormanager/editormanager.cpp \
    dialogs/shortcutsettings.cpp \
    actionmanager/actionmanager.cpp \
    actionmanager/command.cpp \
    actionmanager/actioncontainer.cpp \
    actionmanager/commandsfile.cpp \

HEADERS += \
    core_global.h \
    mainwindow.h \
    coreimpl.h \
    ifilefactory.h \
    uniqueidmanager.h \
    mimedatabase.h \
    outputpane.h \
    editormanager/editormanager.h\
    editormanager/ieditorfactory.h\
    editormanager/ieditor.h\
    actionmanager/actioncontainer.h \
    actionmanager/actionmanager.h \
    actionmanager/command.h \
    actionmanager/actionmanager_p.h \
    actionmanager/command_p.h \
    actionmanager/actioncontainer_p.h \
    actionmanager/commandsfile.h \
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
    dialogs/shortcutsettings.h

FORMS += dialogs/shortcutsettings.ui

