TEMPLATE = lib
CONFIG *= staticlib

TARGET = ProjectExplorer
QT += xml \
    script
include(../../qworkbenchplugin.pri)
include(projectexplorer_dependencies.pri)
include(../../shared/scriptwrapper/scriptwrapper.pri)
HEADERS += projectexplorer.h \
    session.h \
    projectexplorerconstants.h \
    project.h \
    environment.h \
    runconfiguration.h \
    applicationlauncher.h \
    applicationrunconfiguration.h \
    persistentsettings.h

SOURCES += projectexplorer.cpp \
    session.cpp \
    project.cpp \
    environment.cpp \
    runconfiguration.cpp \
    persistentsettings.cpp

DEFINES += PROJECTEXPLORER_LIBRARY
