import os
import shlex
import shutil
import platform
import subprocess
import commands
import glob

from Project import *

# Tools

def isLinuxOS():
    return platform.system() == 'Linux'

def isMacOS():
    return platform.system() == 'MacOS'

def isWindowsOS():
    return platform.system() == 'Windows'

def banner(string, ch = '=', length = 80):
    text = ' %s ' % ( string )
    text = text.center( length, ch )
    print ch *length
    print text
    print ch *length

def deleteIfExists(filePath):
    if filePath == os.environ[ 'HOME' ]:
        return False
    if not os.path.exists( filePath ):
        return True
    if os.path.isdir( filePath ) and not os.path.islink( filePath ):
        shutil.rmtree( filePath )
    else:
        os.remove( filePath )
    return not os.path.exists( filePath )

def createDirectory(filePath):
    if os.path.exists( filePath ):
        if os.path.isdir( filePath ):
            return True
        else:
            return False
    os.makedirs( filePath )
    return os.path.exists( filePath )

def createSymLink(source, target):
    if os.path.exists( target ):
        if os.path.islink( target ):
            return os.path.realpath( target ) == source
        return False
    os.symlink( source, target )
    return os.path.exists( target )

def execute(command, workingDirectory = None, showError = True):
    if workingDirectory:
        print ' - From: %s' % ( workingDirectory )
    print ' - Executing: %s' % command
    args = shlex.split( command )
    runner = subprocess.Popen( args, cwd = workingDirectory, stdout = subprocess.PIPE, stderr= subprocess.STDOUT, shell = False )
    output, error = runner.communicate()
    if runner.returncode != 0:
        if showError:
            print ' - Exit Code: %i' % ( runner.returncode )
            print output
        return False
    return True

def tarGzFolder(source, target, options = None):
    directory = os.path.dirname( source )
    folderName = os.path.basename( source )
    return execute( 'tar czf "%s" "%s"' % ( target, folderName ), directory )

def zipFolder(source, target, options = None, exclude = None, include = None):
    directory = os.path.dirname( source )
    folderName = os.path.basename( source )
    return execute( 'zip -q -r -9 %s "%s" "%s" %s %s' % ( '"%s"' % ( options ) if options else '', target, folderName, '-x %s' % ( exclude ) if exclude else '', '-i %s' % ( include ) if include else '' ), directory )

def buildQtProject(projectFilePath, qtHost, qtMkSpec):
    directory = os.path.dirname( projectFilePath )
    filename = os.path.basename( projectFilePath )
    jobs = os.environ[ 'PROJECT_MAKE_JOBS' ]
    make = 'make -j %s' % ( jobs if jobs else '1' )
    qmake = '%s %s "%s"' % ( qtHost.bin( 'qmake' ), '-spec "%s"' %( qtMkSpec ) if qtMkSpec else '', filename )
    
    if not os.path.exists( directory ):
        return False
    
    # not important if it fails
    execute( '%s distclean' % ( make ), directory, False )
    
    if not execute( qmake, directory ):
        return False
    
    if not execute( '%s distclean' % ( make ), directory ):
        return False
    
    if not execute( qmake, directory ):
        return False
    
    if not execute( make, directory ):
        return False
    
    return True

# Wine

class Wine:
    def __init__(self):
        if isLinuxOS():
            self.wine = 'wine'
            self.prefix = '%s/.wine' % ( os.environ[ 'HOME' ] )
        elif isMacOS():
            self.wine = '/Applications/Wine.app/Contents/Resources/bin/wine'
            self.prefix = '%s/Wine Files' % ( os.environ[ 'HOME' ] )
        elif isWindowsOS():
            '''No wine needed'''
        
        self.rootDrive = 'z:'
        self.drive = '%s/drive_c' % ( self.prefix )
        self.programFiles = '%s/Program Files' % ( self.drive )
    
    def start(self, command, workingDirectory = None):
        return execute( '"%s" %s' % ( self.wine, command ), workingDirectory )
    
    def iscc(self, scriptFile, workingDirectory = None):
        return self.start( '"%s/Inno Setup 5/ISCC.exe" "%s"' % ( self.programFiles, scriptFile ), workingDirectory )
    
    def isccInstall(self, filePath):
        if not os.path.exists( filePath ):
            return False
        return self.start( '"%s%s" /silent' % ( self.rootDrive, filePath ) )
    
    def isccUninstall(self, filePath):
        if not os.path.exists( filePath ):
            return False
        if os.path.isdir( filePath ):
            for file in glob.glob( '%s/unins*.exe' % ( filePath ) ):
                if not self.isccUninstall( file ):
                    return False
            return True
        else:
            return self.start( '"%s%s" /silent' % ( self.rootDrive, filePath ) )
    
    def isccSetupToZip(self, setupFilePath, zipFilePath, defaultInstallDirectory):
        pf = '%s/%s' % ( self.programFiles, defaultInstallDirectory )
        sl = os.path.splitext( zipFilePath )[ 0 ]
        self.isccUninstall( pf )
        if not self.isccInstall( setupFilePath ):
            return False
        ok = createSymLink( pf, sl )
        if ok:
            ok = zipFolder( sl, zipFilePath, exclude = '*unins*.*' )
        deleteIfExists( sl )
        self.isccUninstall( pf )
        return ok
    
    def expandVariables(self):
        os.environ[ 'WINE_BINARY' ] = self.wine
        os.environ[ 'WINEPREFIX' ] = self.prefix # official used variable by wine binary
        os.environ[ 'WINE_PREFIX' ] = self.prefix
        os.environ[ 'WINE_ROOT_DRIVE' ] = self.rootDrive
        os.environ[ 'WINE_DRIVE' ] = self.drive
        os.environ[ 'WINE_PROGRAM_FILES' ] = self.programFiles

# Svn

class Svn:
    def __init__(self, url, workingCopy = None, login = None, password = None):
        self.url = url
        self.workingCopy = workingCopy
        self.login = login
        self.password = password
    
    def checkout(self, url, filePath, fromWorkingCopy = False):
        return Svn.staticCheckout( '%s/%s' % ( self.workingCopy, url ), filePath )
    
    def export(self, url, filePath, fromWorkingCopy = False):
        return Svn.staticExport( '%s/%s' % ( self.workingCopy if fromWorkingCopy else self.url, url ), filePath )
    
    @staticmethod
    def staticCheckout(url, filePath):
        return False
    
    @staticmethod
    def staticExport(url, filePath):
        deleteIfExists( filePath )
        return execute( 'svn export "%s" "%s"' % ( url, filePath ) )
    
    def workingCopyRevision(filePath):
        status, output = commands.getstatusoutput( 'export LANG=C && [ -f /usr/bin/svnversion ] && svnversion -c "$1" | sed "s/[^:]*:\([0-9]*\)[a-zA-Z]*/\1/"' )
        return output if os.WEXITSTATUS( status ) == 0 else '#Error'

# Git

class Git:
    def __init__(self, url, login = None, password = None):
        self.url = url
        self.login = login
        self.password = password

# QtTriplet

class QtTriplet:
    def __init__(self, linux = None, macos = None, windows = None):
        self.linux = linux;
        self.macos = macos;
        self.windows = windows;

# QtHost

class QtHost:
    suffixedBinaries = [ 'assistant', 'linguist', 'lupdate', 'pinentry', 'qtconfig', 'designer', 'lrelease', 'moc', 'qmake', 'uic' ]
    
    def __init__(self, project):
        self.useQtSuffix = True if isLinuxOS() else False
        
        if isLinuxOS():
            self.qt = project.qtLinux
            self.qtHost = self.qt.linux;
        elif isMacOS():
            self.qt = project.qtMac
            self.qtHost = self.qt.macos;
        elif isWindowsOS():
            self.qt = project.qtWindows
            self.qtHost = self.qt.windows;
        
        # these can be overrides if needed
        self.linuxMkSpec = '%s/.qt/mkspecs/linux-x11-g++' % ( os.environ[ "HOME" ] )
        self.macosMkSpec = '%s/.qt/mkspecs/macx-x11-g++42' % ( os.environ[ "HOME" ] )
        self.windowsMkSpec = '%s/.qt/mkspecs/win32-x11-g++' % ( os.environ[ 'HOME' ] )
    
    def expandVariables(self):
        os.environ[ 'QT_LINUX_PATH' ] = self.qt.linux
        os.environ[ 'QT_MACOS_PATH' ] = self.qt.macos
        os.environ[ 'QT_WINDOWS_PATH' ] = self.qt.windows
    
    def suffix(self, command):
        return '-qt4' if self.useQtSuffix and command in QtHost.suffixedBinaries else ''
    
    def bin(self, command):
        return '%s/bin/%s%s' % ( self.qtHost, command, self.suffix( command ) )
