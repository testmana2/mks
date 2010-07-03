# Monkey Studio 2 qCtagsSense project include file

# needed dependencies
include( $${PWD}/../ctags/ctags.pri )

# include path
INCLUDEPATH	*= $$getFolders( $${PWD}/src )

# dependency
PRE_TARGETDEPS	*= $${PWD}

# library integration
LIBS	*= -L$${PACKAGE_BUILD_PATH}

CONFIG(debug, debug|release) {
	#Debug
	unix:LIBS	*= -l$${TARGET}_debug
	else:LIBS	*= -l$${TARGET}d
} else {
	#Release
	LIBS	*= -l$${TARGET}
}
