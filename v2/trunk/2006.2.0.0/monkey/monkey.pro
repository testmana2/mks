warning( "This file has been generated by Monkey Studio (2006.1.6.0) - http://sourceforge.net/projects/monkeystudio" )

unix:OBJECTS_DIR	= ../build/.o/unix
win32:OBJECTS_DIR	= ../build/.o/win32
mac:OBJECTS_DIR	= ../build/.o/mac
UI_DIR	= ../build/.ui
MOC_DIR	= ../build/.moc
RCC_DIR	= ../build/.rcc

mac:ICON	+= src/resources/Icons/icon.icns
unix:ICON	+= src/resources/Icons/icon.png
win32:ICON	+= src/resources/Icons/icon.ico

TEMPLATE	= app
DEFINES	= MONKEY_CORE_BUILD
LIBS	= -lqscintilla2
win32:LIBS	+= -Wl,--out-implib,libmonkey.a
unix:LIBS	+= -rdynamic
QT	= gui core
LANGUAGE	= C++
INCLUDEPATH	= . src src/ui src/3rdparty/objects/pSettings/ src/3rdparty/widgets/pMenuBar/ src/workspacemanager/ src/maininterface/ src/toolsmanager/ui/ src/3rdparty/widgets/pTabToolBar/ src/3rdparty/widgets/pTabWorkspace/ src/projectsmanager/ src/pluginsmanager/ src/maininterface/ui/ src/toolsmanager/ src/recentsmanager/ src/monkeyeditor/ui/
CONFIG	+= qt warn_on release app_bundle thread x11 windows console
DESTDIR	= ../binary
win32:RC_FILE	+= monkey.rc
RESOURCES	= src/resources/resources.qrc

SOURCES	= src/3rdparty/objects/pSettings/pSettings.cpp \
	src/3rdparty/widgets/pMenuBar/pMenuBar.cpp \
	src/3rdparty/widgets/pTabWorkspace/pTabWorkspace.cpp \
	src/3rdparty/widgets/pTabWorkspace/pRightCorner.cpp \
	src/3rdparty/widgets/pTabToolBar/kmultitabbar.cpp \
	src/3rdparty/widgets/pTabToolBar/pTabToolBar.cpp \
	src/3rdparty/widgets/pTabToolBar/pTabToolBarManager.cpp \
	src/maininterface/Settings.cpp \
	src/maininterface/MenuBar.cpp \
	src/maininterface/StatusBar.cpp \
	src/maininterface/ui/UITranslator.cpp \
	src/recentsmanager/RecentsManager.cpp \
	src/toolsmanager/ToolsManager.cpp \
	src/toolsmanager/ui/UIToolsEdit.cpp \
	src/maininterface/ui/UIAbout.cpp \
	src/maininterface/ui/UIMain.cpp \
	src/monkeyeditor/ui/qscintillaSearch.cpp \
	src/monkeyeditor/MonkeyEditor.cpp \
	src/workspacemanager/UISaveFiles.cpp \
	src/workspacemanager/TabToolBar.cpp \
	src/workspacemanager/Workspace.cpp \
	src/pluginsmanager/PluginsManager.cpp \
	src/projectsmanager/ProjectsManager.cpp \
	src/Monkey.cpp \
	src/main.cpp \
	src/workspacemanager/LeftCorner.cpp \
	src/workspacemanager/FileType.cpp

FORMS	= src/maininterface/ui/UITranslator.ui \
	src/toolsmanager/ui/UIToolsEdit.ui \
	src/maininterface/ui/UIAbout.ui \
	src/maininterface/ui/UIMain.ui \
	src/monkeyeditor/ui/qscintillaSearch.ui

OPENEDFILES	= src/pluginsmanager/ChildPlugin.h \
	src/monkeyeditor/MonkeyEditor.h \
	src/monkeyeditor/MonkeyEditor.cpp \
	src/workspacemanager/AbstractChild.h \
	src/workspacemanager/Workspace.h \
	src/workspacemanager/Workspace.cpp

HEADERS	= src/3rdparty/objects/pSettings/pSettings.h \
	src/3rdparty/widgets/pMenuBar/pMenuBar.h \
	src/3rdparty/widgets/pTabWorkspace/pTabWorkspace.h \
	src/3rdparty/widgets/pTabWorkspace/pRightCorner.h \
	src/3rdparty/widgets/pTabToolBar/kmultitabbar.h \
	src/3rdparty/widgets/pTabToolBar/pTabToolBar.h \
	src/3rdparty/widgets/pTabToolBar/pTabToolBarManager.h \
	src/maininterface/Settings.h \
	src/maininterface/MenuBar.h \
	src/maininterface/StatusBar.h \
	src/maininterface/ui/UITranslator.h \
	src/recentsmanager/RecentsManager.h \
	src/toolsmanager/ToolsManager.h \
	src/toolsmanager/ui/UIToolsEdit.h \
	src/maininterface/ui/UIAbout.h \
	src/maininterface/ui/UIMain.h \
	src/monkeyeditor/ui/qscintillaSearch.h \
	src/monkeyeditor/MonkeyEditor.h \
	src/workspacemanager/AbstractChild.h \
	src/workspacemanager/UISaveFiles.h \
	src/workspacemanager/TabToolBar.h \
	src/workspacemanager/Workspace.h \
	src/pluginsmanager/MonkeyExport.h \
	src/pluginsmanager/BasePlugin.h \
	src/pluginsmanager/PluginsManager.h \
	src/pluginsmanager/WorkspacePlugin.h \
	src/pluginsmanager/ChildPlugin.h \
	src/pluginsmanager/CompilerPlugin.h \
	src/pluginsmanager/DebuggerPlugin.h \
	src/projectsmanager/ProjectsManager.h \
	src/Monkey.h \
	src/main.h \
	src/workspacemanager/LeftCorner.h \
	src/workspacemanager/FileType.h

