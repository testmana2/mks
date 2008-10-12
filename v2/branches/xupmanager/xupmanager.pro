ProjectSettings {
	EDITOR	= QMakeProjectEditor
	QT_VERSION	= 4.4.0
	EXECUTE_DEBUG	= xupmanager
}

# xup framework
TEMPLATE	= app
QT	*= xml

XUP_PWD	= $$PWD

RESOURCES	*= $${XUP_PWD}/resources/XUPManager.qrc \
	addons/qmake/resources/QMake.qrc

FORMS	*= gui/XUPProjectManager.ui

HEADERS	*= $${XUP_PWD}/core/XUPProjectModel.h \
	$${XUP_PWD}/core/XUPItem.h \
	$${XUP_PWD}/core/XUPProjectItem.h \
	iconmanager/pIconManager.h \
	gui/XUPProjectManager.h \
	addons/qmake/QMake2XUP.h \
	addons/qmake/QMakeProjectItem.h \
	core/XUPProjectItemInfos.h

SOURCES	*= $${XUP_PWD}/core/XUPProjectModel.cpp \
	$${XUP_PWD}/core/XUPItem.cpp \
	$${XUP_PWD}/core/XUPProjectItem.cpp \
	$${XUP_PWD}/main.cpp \
	iconmanager/pIconManager.cpp \
	gui/XUPProjectManager.cpp \
	addons/qmake/QMake2XUP.cpp \
	addons/qmake/QMakeProjectItem.cpp \
	core/XUPProjectItemInfos.cpp

SOURCES	+= core/XUPFilteredProjectModel.cpp
HEADERS	+= core/XUPFilteredProjectModel.h