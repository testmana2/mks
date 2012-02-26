import tempfile
import shutil

from Project import *
from Tools import *

class MkSProject(Project):
    def __init__(self):
        Project.__init__(self)
        
        self.success = False
        
        self.shortName = 'mks'
        self.name = 'Monkey Studio IDE'
        #self.version = '1.9.0.0'
        self.version = 'dev'
        self.company = 'The Monkey Studio Team'
        self.copyrights = '2005 - 2012 Azevedo Filipe & The Monkey Studio Team'
        self.description = 'Free, Fast and Flexible cross-platform IDE'
        self.urlHomepage = 'http://monkeystudio.org'
        self.urlForums = '%s/forum' % ( self.urlHomepage )
        self.urlIssuesTracker = 'https://bugs.launchpad.net/monkeystudio/+filebug'
        self.target = '%s/Releases/%s' % ( os.environ[ 'HOME' ], self.shortName )
        
        self.makeJobs = 4
        
        self.qtLinux = QtTriplet(
            '/usr', # linux
            '%s/Developpement/OS/OSX-Libraries/Qt/4.7.4' % ( os.environ[ 'HOME' ] ), # mac
            '%s/Developpement/OS/Win32-Libraries/Qt/4.7.4' % ( os.environ[ 'HOME' ] ) # windows
        )
        
        self.qt = QtHost( self )
        
        self.wine = Wine()
        
        self.svnList[ 'mks' ] = Svn( 'svn://svn.tuxfamily.org/svnroot/monkeystudio/mks' )
        self.svnList[ 'mks' ].workingCopy = '%s/Developpement/C++/Qt4/mks' % ( os.environ[ 'HOME' ] )
        
        self.gitList[ 'fresh' ] = Git( 'git://github.com/pasnox/fresh.git' )
        
        # Custom variables
        self.baseName = '%s_%s' % ( self.shortName, self.version )
        self.sourceName = '%s-src' % ( self.baseName )
        self.tmpFolder = tempfile.mkdtemp()
        #self.tmpFolder = '/tmp/tmpqKUvr3'
        self.logFolder = '%s/log' % ( self.tmpFolder )
        self.packagesFolder = '%s/packages' % ( self.tmpFolder )
        self.sourcesFolder = '%s/%s' % ( self.tmpFolder, self.sourceName )
        self.projectFile = '%s/build.pro' % ( self.sourcesFolder )
        self.tgzFile = '%s/%s.tar.gz' % ( self.packagesFolder, self.sourceName )
        self.zipFile = '%s/%s.zip' % ( self.packagesFolder, self.sourceName )
        self.winSetup = '%s/%s.exe' % ( self.packagesFolder, self.baseName )
        self.winZip = '%s/%s-win32.zip' % ( self.packagesFolder, self.baseName )
        self.macZip = '%s/%s-mac.zip' % ( self.packagesFolder, self.baseName )
        
        '''
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
        '''
    
    def __del__(self):
        if self.success:
            deleteIfExists( self.target )
            createDirectory( self.target )
            for fileName in os.listdir( self.packagesFolder ):
                sourceFilePath = '%s/%s' % ( self.packagesFolder, fileName )
                targetFilePath = '%s/%s' % ( self.target, fileName )
                if not os.path.isdir( sourceFilePath ):
                    if deleteIfExists( targetFilePath ):
                        shutil.move( sourceFilePath, targetFilePath )
        deleteIfExists( self.tmpFolder )
        banner( 'Releasing finished (%s)' % ( 'Ok' if self.success else 'Fails' ) )
    
    def expandVariables(self):
        Project.expandVariables(self)
        os.environ[ 'PROJECT_BASE_NAME' ] = self.baseName
        os.environ[ 'PROJECT_SOURCE_NAME' ] = self.sourceName
        os.environ[ 'PROJECT_TMP_FOLDER' ] = self.tmpFolder
        os.environ[ 'PROJECT_LOG_FOLDER' ] = self.logFolder
        os.environ[ 'PROJECT_PACKAGES_FOLDER' ] = self.packagesFolder
        os.environ[ 'PROJECT_SOURCES_FOLDER' ] = self.sourcesFolder
        os.environ[ 'PROJECT_PROJECT_FILE' ] = self.projectFile
        os.environ[ 'PROJECT_TGZ_FILE' ] = self.tgzFile
        os.environ[ 'PROJECT_ZIP_FILE' ] = self.zipFile
        os.environ[ 'PROJECT_WIN_SETUP' ] = self.winSetup
        os.environ[ 'PROJECT_WIN_ZIP' ] = self.winZip
        os.environ[ 'PROJECT_MAC_ZIP' ] = self.macZip
        
        # Windows Setup expands
        os.environ[ 'QT_WINDOWS_MINGW_DOWNLOAD' ] = 'http://get.qt.nokia.com/qt/source/qt-win-opensource-4.7.4-mingw.exe'
        os.environ[ 'WINDOWS_MINGW_DOWNLOAD' ] = 'http://get.qt.nokia.com/misc/MinGW-gcc440_1.zip'
        os.environ[ 'QT_DOWNLOAD' ] = 'http://qt.nokia.com/downloads'
        os.environ[ 'WINDOWS_DLLS_PATH' ] = '%s/Developpement/OS/Win32-Libraries/bin' % ( os.environ[ 'HOME' ] )
        os.environ[ 'SETUP_ICON' ] = '%s/v2/tools/data_windows/setup.ico' % ( self.svnList[ 'mks' ].workingCopy )
        os.environ[ 'SETUP_LEFT_BANNER' ] = '%s/v2/tools/data_windows/left-banner.bmp' % ( self.svnList[ 'mks' ].workingCopy )
        os.environ[ 'SETUP_TOP_BANNER' ] = '%s/v2/tools/data_windows/top-banner.bmp' % ( self.svnList[ 'mks' ].workingCopy )
        os.environ[ 'SETUP_SOURCE_DIRECTORY' ] = self.sourcesFolder
        os.environ[ 'SETUP_OUTPUT_DIRECTORY' ] = self.packagesFolder
        os.environ[ 'SETUP_OUTPUT_NAME' ] = os.path.splitext( os.path.basename( self.winSetup ) )[ 0 ]
    
    def run(self):
        #Project.run(self)
        self.success = False
        
        banner( 'Releasing "%s" using tmp folder "%s"' % ( self.name, self.tmpFolder ) )
        
        banner( 'Expanding variables...' )
        self.expandVariables()
        
        banner( 'Creating directories...' )
        if os.path.exists( self.target ):
            print 'Target exists and it should not, delete it first.'
            #return False
        createDirectory( self.target )
        createDirectory( self.logFolder )
        createDirectory( self.packagesFolder )
        
        banner( 'Exporting sources...' )
        if self.version == 'dev':
            copy = 'v2/branches/dev'
        elif self.version == 'trunk':
            copy = 'v2/trunk'
        else:
            copy = 'v2/tags/version-%s' % ( self.version )
        if not self.svnList[ 'mks' ].export( copy, self.sourcesFolder, True ):
            print 'Can\'t export sources'
            return
        
        banner( 'Creating sources packages...' )
        if not tarGzFolder( self.sourcesFolder, self.tgzFile ):
            print 'Can\'t create tgz file'
            return
        if not zipFolder( self.sourcesFolder, self.zipFile ):
            print 'Can\'t create zip file'
            return
        
        banner( 'Crossbuilding for windows...' )
        if not buildQtProject( self.projectFile, self.qt, self.qt.windowsMkSpec ):
            print 'Can\'t build for windows'
            return
        
        banner( 'Creating windows setup...' )
        if not self.wine.iscc( '%s%s/v2/tools/data_windows/monkeystudio.iss' % ( self.wine.rootDrive, self.svnList[ 'mks' ].workingCopy ) ):
            print 'Can\'t create windows setup'
            return
        
        banner( 'Creating windows zip...' )
        if not self.wine.isccSetupToZip( self.winSetup, self.winZip, self.name ):
            print 'Can\'t create windows zip'
            return False
        
        banner( 'Crossbuilding for mac os x...' )
        if not buildQtProject( self.projectFile, self.qt, self.qt.macosMkSpec ):
            print 'Can\'t build for mac os x'
            return
        
        '''
        -extract svn
        -create source tgz
        -create source zip
        -windows cross build
        -windows setup
        -windows zip package
        -mac cross build
        mac dmg
        mac zip package
        '''
        
        #print self.qt.bin( 'designer' )
        self.success = True