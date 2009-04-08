TEMPLATE = lib
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
    applicationrunconfiguration.h

SOURCES += projectexplorer.cpp \
    session.cpp \
    project.cpp \
    environment.cpp \
    runconfiguration.cpp \

DEFINES += PROJECTEXPLORER_LIBRARY
