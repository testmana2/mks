###########################################################################################
##		Created using Monkey Studio v1.8.3.2
##
##	Author    : You Name <your@email.org>
##	Project   : classexplorer
##	FileName  : classexplorer.pro
##	Date      : 2009-03-27T22:20:17
##	License   : GPL
##	Comment   : Creating using Monkey Studio RAD
##	Home Page   : http://www.mydomain.org
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

XUPProjectSettings {
	EDITOR	= QMake
	QT_VERSION	= Qt System (4.4.3)
	EXECUTE_DEBUG	= qCtagsSense/qCtagsSense_debug
	EXECUTE_RELEASE	= qintellisense
}

TEMPLATE	= subdirs
SUBDIRS	= ctags qCtagsSense
CONFIG	+= debug_and_release
