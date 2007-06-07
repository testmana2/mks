# 3rd party project file

TEMPLATE	= app

#CONFIG += debug

BUILD_PATH	 = ./build

unix:OBJECTS_DIR	= $$BUILD_PATH/.o/unix
win32:OBJECTS_DIR	= $$BUILD_PATH/.o/win32
mac:OBJECTS_DIR	= $$BUILD_PATH/.o/mac
UI_DIR	= $$BUILD_PATH/.ui
MOC_DIR	= $$BUILD_PATH/.moc
RCC_DIR	= $$BUILD_PATH/.rcc

include( 3rdparty.pri )

HEADER	= MonkeyExport.h

# coveragemeter
#FORMS	+= UITestReport.ui
#HEADERS	+= UITestReport.h
#SOURCES	+= UITestReport.cpp

# objects
# QSingleton
HEADERS	+= QSingleton.h

# widgets
# pTabbedWorkspace
HEADERS	+= pTabbedWorkspaceCorner.h \
	pTabbedWorkspace.h \
	pTabbedWorkspaceRightCorner.h
SOURCES	+= pTabbedWorkspaceCorner.cpp \
	pTabbedWorkspace.cpp \
	pTabbedWorkspaceRightCorner.cpp

contains( TEMPLATE, app ):SOURCES	+= main.cpp