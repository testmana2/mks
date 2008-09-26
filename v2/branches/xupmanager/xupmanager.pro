ProjectSettings {
	EDITOR	= QMakeProjectEditor
	QT_VERSION	= 4.4.0
	EXECUTE_DEBUG	= xupmanager
}

# xup framework
TEMPLATE	= app
QT	*= xml

XUP_PWD	= $$PWD

RESOURCES	*= $${XUP_PWD}/resources/XUPManager.qrc

FORMS	*= gui/XUPProjectManager.ui

HEADERS	*= $${XUP_PWD}/core/XUPProjectModel.h \
	$${XUP_PWD}/core/XUPItem.h \
	$${XUP_PWD}/core/XUPProjectItem.h \
	iconmanager/pIconManager.h \
	gui/XUPProjectManager.h

SOURCES	*= $${XUP_PWD}/core/XUPProjectModel.cpp \
	$${XUP_PWD}/core/XUPItem.cpp \
	$${XUP_PWD}/core/XUPProjectItem.cpp \
	$${XUP_PWD}/main.cpp \
	iconmanager/pIconManager.cpp \
	gui/XUPProjectManager.cpp
