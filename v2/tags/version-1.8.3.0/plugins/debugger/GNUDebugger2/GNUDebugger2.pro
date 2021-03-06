
include( ../../plugins.pri )

DESTDIR	= $$MONKEY_PLUGINS_DIR/GNUdbg2
TARGET	= GNUDebugger2
RESOURCES	= src/resources/GNUDebugger.qrc
QT	+= script

SOURCES	= src/GNUDebugger.cpp \
	src/DockGNUDebugger.cpp \
	src/kernel/gdbProcess.cpp \
	src/ui/UIGNUDebuggerSetting.cpp \
	src/ui/UIManageParsingFile.cpp \
	src/ui/UIManageDelegate.cpp \
	src/kernel/gdbPatternFile.cpp \
	src/kernel/gdbInterpreter.cpp \
	src/kernel/gdbRestoreLine.cpp \
	src/kernel/gdbParser.cpp \
	src/kernel/gdbConnectTemplate.cpp \
	src/kernel/gdbBridgeEditor.cpp \
	src/kernel/gdbKernelDispatcher.cpp \
	src/kernel/gdbCore.cpp \
	src/kernel/gdbSequencer.cpp \
	src/kernel/gdbSetting.cpp \
	src/addon/gdbBreakpoint/gdbBreakpoint.cpp \
	src/addon/gdbBreakpoint/ui/UIGdbBreakpoint.cpp \
	src/addon/gdbBreakpoint/ui/UIBreakpointDelegate.cpp \
	src/addon/gdbBacktrace/gdbBacktrace.cpp \
	src/addon/gdbBacktrace/ui/UIGdbBacktrace.cpp \
	src/addon/gdbRegister/gdbRegister.cpp \
	src/addon/gdbRegister/ui/UIGdbRegister.cpp \
	src/addon/gdbToolTip/gdbToolTip.cpp \
	src/addon/gdbCli/gdbCli.cpp \
	src/addon/gdbScript/gdbScript.cpp \
	src/addon/gdbCli/ui/UIGdbCli.cpp \
	src/addon/gdbWatch/gdbWatch.cpp \
	src/addon/gdbWatch/ui/UIGdbWatch.cpp \
	src/addon/gdbWatch/gdbWatchStruct.cpp

FORMS	= src/ui/UIGNUDebuggerSetting.ui \
	src/ui/UIManageParsingFile.ui \
	src/addon/gdbBreakpoint/ui/UIGdbBreakpoint.ui \
	src/addon/gdbBacktrace/ui/UIGdbBacktrace.ui \
	src/addon/gdbRegister/ui/UIGdbRegister.ui \
	src/addon/gdbWatch/ui/UIGdbWatch.ui \
	src/addon/gdbCli/ui/UIGdbCli.ui

OPENEDFILES	= src/ui/UIsetting.h \
	src/ui/UIsetting.cpp \
	src/GNUDebugger.h \
	src/GNUDebugger.cpp \
	src/addon/gdbBreakpoint/ui/UIGdbBreakpoint.h \
	src/addon/gdbBreakpoint/ui/UIGdbBreakpoint.cpp \
	src/ui/UIManageParsingFile.h \
	src/ui/UIManageParsingFile.cpp \
	src/addon/gdbBacktrace/ui/UIBacktrace.h \
	src/addon/gdbBacktrace/ui/UIBacktrace.cpp

HEADERS	= src/GNUDebugger.h \
	src/DockGNUDebugger.h \
	src/kernel/gdbProcess.h \
	src/ui/UIGNUDebuggerSetting.h \
	src/ui/UIManageParsingFile.h \
	src/ui/UIManageDelegate.h \
	src/kernel/gdbPatternFile.h \
	src/kernel/gdbInterpreter.h \
	src/kernel/gdbRestoreLine.h \
	src/kernel/gdbParser.h \
	src/kernel/gdbConnectTemplate.h \
	src/kernel/gdbBridgeEditor.h \
	src/kernel/gdbKernelDispatcher.h \
	src/kernel/gdbCore.h \
	src/kernel/gdbSequencer.h \
	src/kernel/gdbSetting.h \
	src/addon/gdbBreakpoint/gdbBreakpoint.h \
	src/addon/gdbBreakpoint/gdbBreakpointStruct.h \
	src/addon/gdbBreakpoint/ui/UIGdbBreakpoint.h \
	src/addon/gdbBreakpoint/ui/UIBreakpointDelegate.h \
	src/addon/gdbBacktrace/gdbBacktrace.h \
	src/addon/gdbBacktrace/ui/UIGdbBacktrace.h \
	src/addon/gdbRegister/gdbRegister.h \
	src/addon/gdbRegister/ui/UIGdbRegister.h \
	src/addon/gdbToolTip/gdbToolTip.h \
	src/addon/gdbScript/gdbScript.h \
	src/addon/gdbCli/gdbCli.h \
	src/addon/gdbCli/ui/UIGdbCli.h \
	src/addon/gdbWatch/gdbWatch.h \
	src/addon/gdbWatch/ui/UIGdbWatch.h \
	src/addon/gdbWatch/gdbWatchStruct.h
