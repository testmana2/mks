#!/bin/sh -e

# usefull variables
export START_PWD="$PWD"
export PARALLEL_BUILD="4"

export OS="`uname -s`"
export OS_LINUX="0"
export OS_MAC="0"

# builds from linux host
export DEFAULT_LINUX_QT_VERSION="4.7.4"
export DEFAULT_LINUX_WIN32_QT_VERSION="4.7.4"
export DEFAULT_LINUX_MAC_QT_VERSION="4.7.4"

# build from mac host
export DEFAULT_MAC_QT_VERSION="4.7.4-lgpl"
export DEFAULT_MAC_WIN32_QT_VERSION="4.7.4"
export DEFAULT_MAC_LINUX_QT_VERSION="4.7.4"

export QT_HOST_QMAKE="qmake"
export QT_WIN32_SPEC="$HOME/.qt/win32-x11-g++"
export QT_MAC_SPEC="$HOME/.qt/macx-x11-g++42"

if [ "$OS" = "Linux" ]; then
	export OS_LINUX="1"
	export QT_LINUX_VERSION="$DEFAULT_LINUX_QT_VERSION"
	export QT_WIN32_VERSION="$DEFAULT_LINUX_WIN32_QT_VERSION"
	export QT_MAC_VERSION="$DEFAULT_LINUX_MAC_QT_VERSION"
	
	export QT_HOST_VERSION="$QT_LINUX_VERSION"
	export QT_HOST_QMAKE="qmake-qt4"
fi

if [ "$OS" = "Darwin" ]; then
	export OS_MAC="1"
	export QT_MAC_VERSION="$DEFAULT_MAC_QT_VERSION"
	export QT_WIN32_VERSION="$DEFAULT_MAC_WIN32_QT_VERSION"
	export QT_LINUX_VERSION="$DEFAULT_MAC_LINUX_QT_VERSION"
	
	export QT_HOST_VERSION="$QT_MAC_VERSION"
	export QT_HOST_QMAKE="qmake"
fi

if [ $OS_LINUX = 1 ]; then
	export WINE="wine"
	export WINEPREFIX="$HOME/.wine"
fi

if [ $OS_MAC = 1 ]; then
	export WINE="/Applications/Wine.app/Contents/Resources/bin/wine"
	export WINEPREFIX="$HOME/Wine Files"
fi

export WINE_ROOT_DRIVE="z:"
export WINE_DRIVE="$WINEPREFIX/drive_c"
export WINE_PROGRAM_FILES="$WINE_DRIVE/Program Files"
export ISCC="$WINE_PROGRAM_FILES/Inno Setup 5/ISCC.exe"

##################################################################################
# Functions
##################################################################################

# display a banner
# $1= message
banner() {
    echo ""
    echo "*********************************************************************"
    printf "%-.80s\n" "$1"
    echo "*********************************************************************"
    #echo ""
}

# execute command and stop if fails
# $1 = message
# $2 = command
# $3 = logfile
bannerCommand() {
	if [ -z "$3" ] ; then
		echo "\t-$1 -> '$2'"
		eval $2
	else
		echo "\t-$1 -> '$2'"
		echo "\t\t-Content loggued -> '$3'"
		eval $2 > "$3" 2>&1
	fi
}

# create log folder $1
createLogFolder() {
	if [ '!' -d "$1" ]; then
		bannerCommand  \
			"Create log folder" \
			"mkdir -p \"$1\""
	fi
}

# delete existing file/folder $1
deleteIfExists() {
	if [ -f "$1" ]; then
		bannerCommand \
			"Deleting file '$1'" \
			"rm -f \"$1\""
	elif [ -d "$1" ]; then
		bannerCommand \
			"Deleting folder '$1'" \
			"rm -fr \"$1\""
	fi
}

# make symlink
# $1 = source
# $2 = target
createSymlink() {
	if [ -f "$1" ] || [ -d "$1" ]; then
		bannerCommand \
			"Create symlink" \
			"ln -s \"$1\" \"$2\""
	fi
}

# execute windows binary with wine
# $1 = windows binary and parameters
# $2 logfile
wineHost() {
	bannerCommand \
		"Executing wine" \
		"\"$WINE\" $1" \
		"$2"
}

# execute inno setup iscc for creating windows setup
# $1 = iss file & parameters to process
# $2 = logfile
isccHost() {
	wineHost \
		"\"$ISCC\" $1" \
		"$2"
}

# uninstall silently a package
# $1 the package file/folder to uninstall
isUninstall() {
	if [ -d "$1" ]; then
		bannerCommand \
			"Uninstall application" \
			"find \"$1\" -name \"unins*.exe\" -print0 | xargs -0 -I {} \"$WINE\" {} /silent" \
			"/dev/null"
	elif [ -f "$1" ]; then
		bannerCommand \
			"Uninstall application" \
			"\"$WINE\" \"$1\" /silent" \
			"/dev/null"
	fi
}

# install silently a package
# $1 the package file to install
isInstall() {
	if [ -f "$1" ]; then
		bannerCommand \
			"Install application" \
			"\"$WINE\" \"$1\" /silent" \
			"/dev/null"
	fi
}

# qmake for host build
# $1 = arguments
# $2 = logfile
qmakeHost() {
	bannerCommand \
		"QMake ($QT_HOST_VERSION) for host" \
		"\"$QT_HOST_QMAKE\" -r $1" \
		"$2"
}

# qmake for windows crossbuild
# $1 = arguments
# $2 = logfile
qmakeWindows() {
	bannerCommand \
		"QMake ($QT_WIN32_VERSION) for windows" \
		"\"$QT_HOST_QMAKE\" -r -spec \"$QT_WIN32_SPEC\" $1" \
		"$2"
}

# qmake for mac crossbuild
# $1 = arguments
# $2 = logfile
qmakeMac() {
	bannerCommand \
		"QMake ($QT_MAC_VERSION) for mac" \
		"\"$QT_HOST_QMAKE\" -r -spec \"$QT_MAC_SPEC\" $1" \
		"$2"
}

# make for host
# $1 = message
# $2 = arguments
# $3 = logfile
makeHost() {
	bannerCommand \
		"$1" \
		"make -j $PARALLEL_BUILD $2" \
		"$3"
}

# make a windows crossbuild
# $1 the qmake project file to build
crossbuildWindows() {
	directory=`dirname "$1"`
	filename=`basename "$1"`
	
	if [ ! -d "$directory" ]; then
		return 1
	fi
	
	current="$PWD"
	
	bannerCommand \
		"Going into sources directory" \
		"cd \"$directory\""
		
	makeHost \
		"Cleaning" \
		"distclean" \
		"/dev/null"
	
	
	qmakeWindows \
		"\"$filename\"" \
		"/dev/null"
	
	makeHost \
		"Cleaning" \
		"distclean" \
		"/dev/null"
	
	qmakeWindows \
		"\"$filename\"" \
		"/dev/null"
	
	makeHost \
		"Building" \
		"" \
		"$START_PWD/log/windowsBuild.log"
	
	bannerCommand \
		"Coming back to original directory" \
		"cd \"$current\""
}

# make a mac os x crossbuild
# $1 the qmake project file to build
crossbuildMac() {
	directory=`dirname "$1"`
	filename=`basename "$1"`
	
	if [ ! -d "$directory" ]; then
		return 1
	fi
	
	current="$PWD"
	
	bannerCommand \
		"Going into sources directory" \
		"cd \"$directory\""
		
	makeHost \
		"Cleaning" \
		"distclean" \
		"/dev/null"
	
	
	qmakeMac \
		"\"$filename\"" \
		"/dev/null"
	
	makeHost \
		"Cleaning" \
		"distclean" \
		"/dev/null"
	
	qmakeMac \
		"\"$filename\"" \
		"/dev/null"
	
	makeHost \
		"Building" \
		"" \
		"$START_PWD/log/macBuild.log"
	
	bannerCommand \
		"Coming back to original directory" \
		"cd \"$current\""
}

# get svn revision from working copy directory
# $1 = directory to get revision from
svnRevision() {
	export LANG=C && [ -f /usr/bin/svnversion ] && svnversion -c "$1" | sed "s/[^:]*:\([0-9]*\)[a-zA-Z]*/\1/"
}

# export a svn path from source $1 to target $2
svnExport() {
	bannerCommand \
		"Exporting repository '$1' to '$2'" \
		"svn export \"$1\" \"$2\"" \
		"/dev/null"
}

# clone a git path from source $1 to target $2
gitExport() {
	bannerCommand \
		"Exporting repository '$1' to '$2'" \
		"git --git-dir=\"$1\" checkout-index -a -f --prefix=\"$2\"" \
		"/dev/null"
}

# create a tar.gz file $1 from path $2
createTarGz() {
	bannerCommand \
		"Creating tar.gz package '$1'" \
		"tar czf \"$1\" \"$2\""
}

# create a zip file
# $1 = zip file
# $2 = from path
# $3 = extra parameters
# $4 = ending parameters
createZip() {
	params=
	
	if [ '!' -z "$3" ]; then
		params="$params \"$3\""
	fi
	
	if [ '!' -z "$1" ]; then
		params="$params \"$1\""
	fi
	
	if [ '!' -z "$2" ]; then
		params="$params \"$2\""
	fi
	
	if [ '!' -z "$4" ]; then
		params="$params $4"
	fi
	
	bannerCommand \
		"Creating zip package" \
		"zip -q -r -9 $params"
}

# MAC OS X SPECIFIC

# The mac deployment tool also deploys the Qt plugins, according to the following rules:
#	* Debug versions of the plugins are not deployed.
#	* The designer plugins are not deployed.
#	* The Image format plugins are always deployed.
#	* SQL driver plugins are deployed if the application uses the QtSql module.
#	* Script plugins are deployed if the application uses the QtScript module.
#	* The Phonon backend plugin is deployed if the application uses the Phonon module.
#	* The svg icon plugin is deployed if the application uses the QtSvg module.
#	* The accessibility plugin is always deployed.
#	* Accessibility for Qt3Support is deployed if the application uses the Qt3Support module.

macRelinkBinary()
{
    #echo "Striping `basename \"$1\"` binary..."
    # strip libs (-x is max allowable for shared libs)
    #strip -x "${1}"
    
    # set id in target
    if [ ! "x$2" = "x" ] ; then
        setId "${2}" "${1}"
    fi
    
    # get dependencies
    frameworks_path=`getBinaryDependencies "$1"`
    
    # update framework/library paths
    changeBinaryPaths "$1" "$frameworks_path"
    
    # copy dependencies frameworks/libraries
    for framework_path in $frameworks_path ; do
        # get framework
        framework=$(basename "$framework_path")
        
        # get filenames
        source=`getSourceFramework "$framework"`
        target=`getTargetFramework "$framework"`
        
        # copy file if needed
        if [ -e "${source}" ] ; then
            if [ ! -e "${target}" ] ; then
                #echo "Copying & striping `basename \"${source}\"` framework/library..."
                path=`dirname "${target}"`
                mkdir -p "${path}"
                cp -f "${source}" "${target}"
                
                echo
                relinkBinary "${target}" "${framework}"
            fi
        fi
    done
}

