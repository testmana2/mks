include( ../../plugins.inc )

DESTDIR	= $$MONKEY_PLUGINS_DIR/base
TARGET	= Navigator
RESOURCES	= icons/icons.qrc

HEADERS	= src/FilesList.h \
		src/FilesListSettings.h

SOURCES	= src/FilesList.cpp \
		src/FilesListSettings.cpp
