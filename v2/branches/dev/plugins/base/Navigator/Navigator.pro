include( ../../plugins.inc )

QT += core gui
TEMPLATE	= lib
LANGUAGE	= C++
CONFIG	+= warn_on debug lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/base
INCLUDEPATH += ../../../monkey/src/ctags
INCLUDEPATH += ../../../monkey/src/3rdparty/exuberantCtags
RESOURCES	= icons/icons.qrc

SOURCES	= src/Navigator.cpp  \
		src/Entity.cpp  \
		src/EntityContainer.cpp \ 
		src/NavigatorSettings.cpp
		

HEADERS	= src/Navigator.h	\
		src/Entity.h  \
		src/EntityContainer.h \ 
		src/NavigatorSettings.h


