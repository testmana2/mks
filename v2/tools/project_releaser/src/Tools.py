import os
import shlex
import shutil
import platform
import subprocess
import commands
import glob
import re

import Project
import Qt
import Git
import Svn
import Wine

def isLinuxOS():
    return platform.system() == 'Linux'

def isMacOS():
    return platform.system() == 'MacOS'

def isWindowsOS():
    return platform.system() == 'Windows'

def grep(string, list):
    expr = re.compile( string )
    return filter( expr.search, list )

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

def copy(source, target):
    fileName = os.path.basename( source )
    targetFilePath = target
    if not targetFilePath.endswith( fileName ):
        targetFilePath = '%s/%s' % ( target, fileName )
    if os.path.exists( targetFilePath ):
        return True
    shutil.copy2( source, target )
    return os.path.exists( targetFilePath )

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

def executeAndGetOutput(command, workingDirectory = None):
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
    return ( runner.returncode == 0, output )

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
    #execute( '%s distclean' % ( make ), directory, False )
    
    if not execute( qmake, directory ):
        return False
    
    #if not execute( '%s distclean' % ( make ), directory ):
        #return False
    
    if not execute( qmake, directory ):
        return False
    
    if not execute( make, directory ):
        return False
    
    return True
