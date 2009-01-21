###########################################################################################
##		Created using Monkey Studio v1.8.3.0
##
##	Author    : MkS Team <contact@monkeystudio.org>
##	Project   : QGdb
##	FileName  : QGdb.pro
##	Date      : 2009-01-19T20:33:17
##	License   : GPL
##	Comment   : Creating using Monkey Studio IDE
##	Home Page   : http://monkeystudio.org
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

XUPProjectSettings {
	EDITOR	= QMake
	QT_VERSION	= Qt System (4.4.3)
	EXECUTE_DEBUG	= bin/QGdb_debug
}

TEMPLATE	= subdirs
CONFIG	+= debug_and_release

SUBDIRS	= qmigdb qgdb
