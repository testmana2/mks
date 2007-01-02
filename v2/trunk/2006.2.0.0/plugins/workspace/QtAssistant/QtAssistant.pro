include( "../../plugins.inc" )

TEMPLATE	= lib
QT	= gui core xml
LANGUAGE	= C++
INCLUDEPATH	+= 3rdparty/assistant
CONFIG	+= qt warn_on release lib_bundle thread x11 dll plugin
DESTDIR	= $$MONKEY_PLUGINS_DIR$$/workspace
RESOURCES	= 3rdparty/assistant/assistant.qrc

SOURCES	= src/QtAssistant.cpp \
	src/QtAssistantChild.cpp \
	3rdparty/assistant/config.cpp \
	3rdparty/assistant/docuparser.cpp \
	3rdparty/assistant/helpdialog.cpp \
	3rdparty/assistant/helpwindow.cpp \
	3rdparty/assistant/index.cpp \
	3rdparty/assistant/mainwindow.cpp \
	3rdparty/assistant/profile.cpp \
	3rdparty/assistant/tabbedbrowser.cpp \
	3rdparty/assistant/topicchooser.cpp

FORMS	= 3rdparty/assistant/helpdialog.ui \
	3rdparty/assistant/mainwindow.ui \
	3rdparty/assistant/tabbedbrowser.ui \
	3rdparty/assistant/topicchooser.ui

HEADERS	= src/QtAssistant.h \
	src/QtAssistantChild.h \
	3rdparty/assistant/config.h \
	3rdparty/assistant/docuparser.h \
	3rdparty/assistant/helpdialog.h \
	3rdparty/assistant/helpwindow.h \
	3rdparty/assistant/index.h \
	3rdparty/assistant/mainwindow.h \
	3rdparty/assistant/profile.h \
	3rdparty/assistant/tabbedbrowser.h \
	3rdparty/assistant/topicchooser.h

