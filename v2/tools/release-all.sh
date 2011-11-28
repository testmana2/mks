#!/bin/sh

# load usefull functions
. ./functions.sh

# define usefull variables
export PROJECT_NAME="Monkey Studio IDE"
export PROJECT_COPYRIGHTS="2005 - 2011 Azevedo Filipe & The Monkey Studio Team"
export PROJECT_COMPANY="Monkey Studio Team"
export PROJECT_DESCRIPTION="Free, Fast and Flexible cross-platform IDE"
export PROJECT_URL="http://monkeystudio.org"
export PROJECT_FORUMS_URL="http://monkeystudio.org/forum"
export PROJECT_ISSUES_URL="https://bugs.launchpad.net/monkeystudio/+filebug"

export QT_WIN32_MINGW_DOWNLOAD="http://get.qt.nokia.com/qt/source/qt-win-opensource-4.7.4-mingw.exe"
export WIN32_MINGW_DOWNLOAD="http://get.qt.nokia.com/misc/MinGW-gcc440_1.zip"
export QT_DOWNLOAD="http://qt.nokia.com/downloads"

export QT_WIN32_PATH="$WINE_DRIVE/Development/Qt/$QT_WIN32_VERSION"
export WIN32_DLLS_PATH="$HOME/Win32-Libraries/bin"

# helper variables
APPLICATION_NAME="Monkey Studio"
MAC_BUNDLE_NAME="Monkey Studio"

PROJECT_VERSION="$1"
SOURCE_FOLDER="$START_PWD/../tags/version-$PROJECT_VERSION"

# if empty version
if [ -z "$PROJECT_VERSION" ]; then
	#SOURCE_FOLDER=../branches/dev
	SOURCE_FOLDER="$START_PWD/../trunk"
	
	PROJECT_VERSION="trunk"
fi

PROJECT_REVISION=`svnRevision "$SOURCE_FOLDER"`

# if empty version
if [ -z "$PROJECT_VERSION" ]; then
	PROJECT_VERSION_STR="trunk-svn$PROJECT_REVISION"
# if not empty version
else
	PROJECT_VERSION_STR="$PROJECT_VERSION-svn$PROJECT_REVISION"
fi

BASE_NAME=mks_$PROJECT_VERSION_STR
FOLDER_NAME=$BASE_NAME-src
TAR_GZ_FILE=$FOLDER_NAME.tar.gz
ZIP_FILE=$FOLDER_NAME.zip
WIN_FOLDER=$BASE_NAME-win32
WIN_PACKAGE="$START_PWD/$WIN_FOLDER.zip"
MAC_PACKAGE=$BASE_NAME.dmg
CUR_PATH=$PWD
LOG_FOLDER=$CUR_PATH/log

export SETUP_ICON="$WINE_ROOT_DRIVE/$START_PWD/data_windows/setup.ico"
export SETUP_LEFT_BANNER="$WINE_ROOT_DRIVE/$START_PWD/data_windows/left-banner.bmp"
export SETUP_TOP_BANNER="$WINE_ROOT_DRIVE/$START_PWD/data_windows/top-banner.bmp"
export SETUP_SOURCE_DIRECTORY="$WINE_ROOT_DRIVE/$START_PWD/$FOLDER_NAME"
export SETUP_OUTPUT_DIRECTORY="$WINE_ROOT_DRIVE/$START_PWD"
export SETUP_OUTPUT_NAME="mks_$PROJECT_VERSION_STR-win32"

export WIN_SETUP="$START_PWD/$SETUP_OUTPUT_NAME.exe"

export OS
export PROJECT_VERSION
export PROJECT_VERSION_STR
export PROJECT_REVISION
export MAC_PACKAGE

crossbuildMac() {
	echo "*** Crossbuilding mac port..."
	startCommand "cd \"./$FOLDER_NAME\""
	startCommand "makeHost distclean > /dev/null 2>&1" 0
	startCommand "qmakeMac \"./build.pro\" > /dev/null 2>&1"
	startCommand "makeHost distclean > /dev/null 2>&1" 0
	startCommand "qmakeMac > /dev/null 2>&1"
	startCommand "makeHost > \"$START_PWD/log/macBuild.log\" 2>&1"
	#startCommand "makeHost distclean > /dev/null 2>&1" 0
	startCommand "cd \"$START_PWD\""
}

# crossbuild & setup for windows
crossBuild()
{
	echo "*** Crossbuilding for windows"
	
	QMAKE="qmake"

	if [ $OS_LINUX = 1 ]; then
		QMAKE="qmake-qt4"
		QT_VERSION="$DEFAULT_LINUX_QT_VERSION"
		QT_WIN32_VERSION="$DEFAULT_LINUX_WIN32_QT_VERSION"
		QT_PATH="/usr"
		MKSPEC="$HOME/.qt/win32-x11-g++"
		QT_WIN32_PATH="$WINE_DRIVE/Development/Qt/$QT_WIN32_VERSION"
		ISCC="$WINE_PROGRAM_FILES/Inno Setup 5/ISCC.exe"
		DLLS_PATH="$WINE_DRIVE/Development/OpenSSL"
	fi

	if [ $OS_MAC = 1 ]; then
		QT_VERSION="$DEFAULT_MAC_QT_VERSION"
		QT_WIN32_VERSION="$DEFAULT_MAC_WIN32_QT_VERSION"
		QT_PATH="/usr/local/Trolltech/$QT_VERSION"
		MKSPEC="$HOME/mkspecs/4.6.x/win32-osx-g++"
		QT_WIN32_PATH="/usr/local/Trolltech/win32/$QT_WIN32_VERSION"
		ISCC="$WINE_PROGRAM_FILES/Inno_Setup_5/ISCC.exe"
		DLLS_PATH="$HOME/Win32Libraries/bin"

	fi

	export QT_WIN32_PATH
	export QT_WIN32_VERSION
	export DLLS_PATH
	export CROSS_WIN32_QT_PATH="$QT_WIN32_PATH"

	startCommand "cd \"./$FOLDER_NAME\""
	startCommand "rm -fr ./setups"
	startCommand "cp -fr \"$CUR_PATH/../branches/dev/setups\" ."
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "\"$QT_PATH/bin/$QMAKE\" -r -spec \"$MKSPEC\" -win32 -r > /dev/null 2>&1"
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "\"$QT_PATH/bin/$QMAKE\" -r -spec \"$MKSPEC\" -win32 -r > /dev/null 2>&1"
	startCommand "make -j$PARALLEL_BUILD > \"$CUR_PATH/log/winbuild.log\" 2>&1"
	startCommand "\"$WINE\" \"$ISCC\" \"./setups/windows/monkeystudio.iss\" > \"$CUR_PATH/log/winpackage.log\" 2>&1"
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "cd \"$CUR_PATH\""

	if [ -f "./$FOLDER_NAME/setups/$WIN_SETUP" ]; then
		startCommand "mv \"./$FOLDER_NAME/setups/$WIN_SETUP\" \"./\""
	fi
}

# create mac os x package
macPackage()
{
	echo "*** Create Mac OS X package"

	BUNDLE_PATH="./bin"
	BUNDLE_APP_PATH="$BUNDLE_PATH/$MAC_BUNDLE_NAME.app"
	QT_PATH="/usr/local/Trolltech/$DEFAULT_MAC_QT_VERSION"
	QMAKE="$QT_PATH/bin/qmake"
	#QMAKE_FLAGS="\"CONFIG *= universal no_fresh_install\""

	startCommand "cd \"./$FOLDER_NAME\""
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "\"$QMAKE\" $QMAKE_FLAGS -r > /dev/null 2>&1"
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "\"$QMAKE\" $QMAKE_FLAGS -r > /dev/null 2>&1"
	startCommand "make -j$PARALLEL_BUILD > \"$CUR_PATH/log/macbuild.log\" 2>&1"
	startCommand "make install > /dev/null 2>&1"
	startCommand "\"../release-osx.sh\" \"$BUNDLE_APP_PATH\" \"$QMAKE\" > \"$CUR_PATH/log/macpackage.log\" 2>&1"
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "cd \"$CUR_PATH\""

	#if [ -f "./$FOLDER_NAME/$BUNDLE_PATH/$MAC_BUNDLE_NAME.dmg" ]; then
	#	startCommand "mv \"./$FOLDER_NAME/$BUNDLE_PATH/$MAC_BUNDLE_NAME.dmg\" \"./$MAC_PACKAGE\""
	#fi
}
macPackage2()
{
	echo "*** Create Mac OS X package"

	BUNDLE_PATH="./bin"
	BUNDLE_APP_PATH="$BUNDLE_PATH/$MAC_BUNDLE_NAME.app"
	QT_PATH="/usr/local/Trolltech/$DEFAULT_MAC_QT_VERSION"
	#QMAKE_FLAGS="\"CONFIG *= universal no_fresh_install\""

	startCommand "cd \"./$FOLDER_NAME\""
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "\"$QT_PATH/bin/qmake\" $QMAKE_FLAGS -r > /dev/null 2>&1"
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "\"$QT_PATH/bin/qmake\" $QMAKE_FLAGS -r > /dev/null 2>&1"
	startCommand "make -j$PARALLEL_BUILD > \"$CUR_PATH/log/macbuild.log\" 2>&1"
	startCommand "make install > /dev/null 2>&1"
	startCommand "\"$QT_PATH/bin/macdeployqt\" \"$BUNDLE_APP_PATH\" -dmg > /dev/null 2>&1"
	startCommand "make distclean > /dev/null 2>&1" 0
	startCommand "cd \"$CUR_PATH\""

	if [ -f "./$FOLDER_NAME/$BUNDLE_PATH/$MAC_BUNDLE_NAME.dmg" ]; then
		startCommand "mv \"./$FOLDER_NAME/$BUNDLE_PATH/$MAC_BUNDLE_NAME.dmg\" \"./$MAC_PACKAGE\""
	fi
}

# finish function, must not use startCommand or function calling it to avoid possible unfinite loop on errors.
finish()
{
	# close wine, WineBottler & X11
	if [ $OS_MAC = 1 ]; then
		startCommand "killall wine > /dev/null 2>&1" 0
		startCommand "killall WineBottler > /dev/null 2>&1" 0
		startCommand "killall X11.bin > /dev/null 2>&1" 0
	fi
	
	# come back to start folder
	startCommand "cd \"$CUR_PATH\"" 0
	
	# delete exported repository
	#startCommand "rm -fr \"./$FOLDER_NAME\"" 0

	echo "********** Processing release finished - Exit code: $1 **********"
	
	exit $1
}

















# create windows zip package
windowsZipPackage()
{
	install_directory="$WINE_PROGRAM_FILES/$PROJECT_NAME"
	
	isUninstall "$install_directory"
	isInstall "$WIN_SETUP"
	
	createSymlink "$install_directory" "$WIN_FOLDER"
	
	createZip \
		"$WIN_PACKAGE" \
		"$WIN_FOLDER" \
		"" \
		"-x *unins*.exe -x *unins*.dat"
	
	deleteIfExists "$WIN_FOLDER"

	isUninstall "$install_directory"
}

banner "Releasing '$APPLICATION_NAME' $PROJECT_VERSION/r$PROJECT_REVISION..."
deleteIfExists "$LOG_FOLDER"
createLogFolder "$LOG_FOLDER"

banner "deleting old stuff..."
#deleteIfExists "./$FOLDER_NAME"
deleteIfExists "./$TAR_GZ_FILE"
deleteIfExists "./$ZIP_FILE"
#deleteIfExists "./$WIN_SETUP"
deleteIfExists "./$WIN_PACKAGE"
deleteIfExists "./$MAC_PACKAGE"

banner "Getting sources..."
#svnExport "./$SOURCE_FOLDER" "./$FOLDER_NAME"

banner "Creating sources packages..."
#createTarGz "./$TAR_GZ_FILE" "./$FOLDER_NAME"
#createZip "./$ZIP_FILE" "./$FOLDER_NAME"

banner "Crossbuilding windows port..."
#crossbuildWindows "./$FOLDER_NAME/build.pro"

banner "Creating windows setup..."
#isccHost "\"./data_windows/monkeystudio.iss\""

banner "Creating windows package..."
windowsZipPackage

exit 0

if [ $OS_LINUX = 1 ]; then
	# create mac build
	echo "lol"
	#crossbuildMac
fi

# create win setup
#crossBuild
# create windows zip package

# finish call
#finish 0