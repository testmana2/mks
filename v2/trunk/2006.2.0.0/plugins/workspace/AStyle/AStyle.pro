include( ../../plugins.inc )

TEMPLATE	= lib
LANGUAGE	= C++
CONFIG	+= warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR/workspace
DEFINES	+= NDEBUG
INCLUDEPATH	+= src src/ui src/3rdparty/astyle

FORMS	= src/ui/UISettingsAStyle.ui

HEADERS	= src/3rdparty/astyle/compiler_defines.h \
	src/3rdparty/astyle/astyle.h \
	src/ui/UISettingsAStyle.h \
	src/FormatterSettings.h \
	src/AStylePlugin.h

SOURCES	= src/3rdparty/astyle/ASBeautifier.cpp \
	src/3rdparty/astyle/ASFormatter.cpp \
	src/ui/UISettingsAStyle.cpp \
	src/FormatterSettings.cpp \
	src/AStylePlugin.cpp
