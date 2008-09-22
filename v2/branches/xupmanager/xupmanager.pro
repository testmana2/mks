# xup framework
TEMPLATE	= app

XUP_PWD	= $$PWD

QT	*= xml
#RESOURCES	*= $${XUP_PWD}/resources/XUPManager.qrc



SOURCES	*= $${XUP_PWD}/main.cpp \
	core/XUPProjectModel.cpp \
	core/XUPItem.cpp

ProjectSettings {
	EDITOR	= QMakeProjectEditor
	QT_VERSION	= 4.4.0
	EXECUTE_DEBUG	= xupmanager
}
HEADERS	*= core/XUPProjectModel.h \
	core/XUPItem.h