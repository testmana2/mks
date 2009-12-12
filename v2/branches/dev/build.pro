XUPProjectSettings {
	QT_VERSION	= Qt System (4.5.2)
	OTHERS_PLATFORM_TARGET_DEBUG	= bin/monkeystudio_debug
	MAC_PLATFORM_TARGET_RELEASE	= bin/Monkey Studio.app
	OTHERS_PLATFORM_TARGET_RELEASE	= bin/monkeystudio
	MAC_PLATFORM_TARGET_DEBUG	= bin/Monkey Studio_debug.app
}

# include install script
include( installs.pri )

TEMPLATE	= subdirs

CONFIG	+= ordered

SUBDIRS	= fresh \
	ctags \
	qCtagsSense \
	monkey \
	plugins

!isEqual( SYSTEM_QSCINTILLA, 1 ) {
	SUBDIRS	= qscintilla $$SUBDIRS
}
