include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/project
TARGET	= NoProject
INCLUDEPATH	+= src/ui

FORMS	= src/ui/UINoProjectProjectSettings.ui

HEADERS = src/NoProject.h \
	src/NoProjectProjectItem.h \
	src/ui/UINoProjectProjectSettings.h

SOURCES	= src/NoProject.cpp \
	src/NoProjectProjectItem.cpp \
	src/ui/UINoProjectProjectSettings.cpp 