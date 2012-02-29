import os

import Project
import Tools
import Qt
import Git
import Svn
import Wine

class MacOSBundle:
    suffixedBinaries = [ 'ld', 'otool' ]
    
    def __init__(self, qtHost, bundlePath):
        self.qt = qtHost
        self.bundlePath = bundlePath
        self.binariesPath = '/opt/mac/bin' if Tools.isLinuxOS() else '/usr/bin'
        self.binariesPrefix = 'x86_64-apple-darwin10-' if Tools.isLinuxOS() else ''
        self.use64BitsBinaries = True if Tools.isLinuxOS() else False
    
    def suffix(self, command):
        return '64' if self.use64BitsBinaries and command in MacOSBundle.suffixedBinaries else ''
    
    def bin(self, command):
        return '%s/%s%s%s' % ( self.binariesPath, self.binariesPrefix, command, self.suffix( command ) )
    
    def frameworksFilePath(self):
        return '%s/Contents/Frameworks' % ( self.bundlePath )
    
    def resourcesFilePath(self):
        return '%s/Contents/Resources' % ( self.bundlePath )
    
    def qtPluginsFilePath(self, name = None):
        return '%s/Contents/Resources/qt/plugins/%s' % ( self.bundlePath, name if name else '' )
    
    def binaryFilePath(self):
        fileName = os.path.splitext( os.path.basename( self.bundlePath ) )[ 0 ]
        return '%s/Contents/MacOS/%s' % ( self.bundlePath, fileName )
    
    def getDependencies(self, binary):
        command = '"%s" -LX "%s"' % ( self.bin( 'otool' ), binary )
        ok, output = Tools.executeAndGetOutput( command )
        items = []
        
        for line in output.splitlines():
            line = line.strip();
            tmp = line.lower()
            if tmp.startswith( '/usr/lib' ) and not 'qt' in tmp and not 'phonon' in tmp:
                continue
            index = line.rfind( '(compatibility' )
            items.append( line[ :index ].strip() )
        
        return items;
    
    def copyDependencies(self, binary, target):
        if not Tools.createDirectory( target ):
            return False
        frameworks = self.getDependencies( binary )
        for framework in frameworks:
            if not os.path.exists( framework ) and ( 'Qt' in framework or 'phonon' in framework ):
                frameworkName = os.path.basename( framework )
                index = framework.find( '.framework' )
                framework = '%s/lib/%s%s' % ( self.qt.qt.macos, frameworkName, framework[ index: ] )
            if not Tools.copy( framework, target ):
                return False
        return True
    
    def deploy(self):
        frameworksPath = self.frameworksFilePath()
        if not self.copyDependencies( self.binaryFilePath(), frameworksPath ):
            return False
        return True
