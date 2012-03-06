import os

import Project
import Tools
import Qt
import Git
import Svn
import Wine

class MacOSBundle:
    suffixedBinaries = [ 'ld', 'otool' ]
    pluginsMapping = {
        'Qt3Support': [ 'accessible' ],
        'QtCore': [ 'codecs', 'sqldrivers' ],
        'QtGui': [ 'accessible', 'graphicssystems', 'iconengines', 'imageformats', 'inputmethods', 'menubar', 'styles' ],
        'QtXml': [  ],
        'QtNetwork': [ 'bearer' ],
        'QtSql': [  ],
        'QtWebKit': [  ],
        'QtOpenGL': [  ],
        'QtDeclarative': [ 'qmltooling' ],
        'QtDesigner': [ 'designer' ],
        'QtDesignerComponents': [  ],
        'QtHelp': [  ],
        'QtXmlPatterns': [  ],
        'QtScript': [ 'script' ],
        'QtScriptTools': [  ],
        'QtDBus': [  ],
        'QtSvg': [  ],
        'QtCLucene': [  ],
        'QtTest': [  ],
        'phonon': [ 'phonon_backend' ]
    }
    
    def __init__(self, qtHost, bundlePath, dropPlugins = True):
        self.qt = qtHost
        self.bundlePath = bundlePath
        self.binariesPath = '/opt/mac/bin' if Tools.isLinuxOS() else '/usr/bin'
        self.binariesPrefix = 'x86_64-apple-darwin10-' if Tools.isLinuxOS() else ''
        self.use64BitsBinaries = True if Tools.isLinuxOS() else False
        self.dropPlugins = dropPlugins
    
    def suffix(self, command):
        return '64' if self.use64BitsBinaries and command in MacOSBundle.suffixedBinaries else ''
    
    def bin(self, command):
        return '%s/%s%s%s' % ( self.binariesPath, self.binariesPrefix, command, self.suffix( command ) )
    
    def frameworksFilePath(self):
        return '%s/Contents/Frameworks' % ( self.bundlePath )
    
    def resourcesFilePath(self):
        return '%s/Contents/Resources' % ( self.bundlePath )
    
    def qtPluginsFilePath(self, name = None):
        return '%s/qt/plugins/%s' % ( self.resourcesFilePath(), name if name else '' )
    
    def binaryFilePath(self):
        fileName = os.path.splitext( os.path.basename( self.bundlePath ) )[ 0 ]
        return '%s/Contents/MacOS/%s' % ( self.bundlePath, fileName )
    
    def getDependencies(self, binary):
        binaryFilePath = os.path.basename( binary )
        command = '"%s" -LX "%s"' % ( self.bin( 'otool' ), binary )
        ok, output = Tools.executeAndGetOutput( command )
        items = []
        for line in set( output.splitlines() ):
            line = line.strip();
            tmp = line.lower()
            if tmp.startswith( '/usr/lib' ) or tmp.startswith( '/system/library' ):
                continue
            index = line.rfind( '(compatibility' )
            filePath = line[ :index ].strip()
            if binaryFilePath != os.path.basename( filePath ):
                items.append( filePath )
        return items
    
    def copyDependencies(self, binary, target):
        if not Tools.createDirectory( target ):
            return False
        frameworks = self.getDependencies( binary )
        
        #print 'Frameworks for %s' % ( binary )
        #print frameworks
        
        for framework in frameworks:
            if framework == os.path.basename( binary ):
                continue
            if not os.path.exists( framework ) and ( 'Qt' in framework or 'phonon' in framework ):
                frameworkName = os.path.basename( framework )
                index = framework.find( '.framework' )
                framework = '%s/lib/%s%s' % ( self.qt.qt.macos, frameworkName, framework[ index: ] )
            if not Tools.copy( framework, target ):
                return False
        return True
    
    def deployPlugins(self):
        frameworkFilePath = self.frameworksFilePath()
        frameworks = os.listdir( frameworkFilePath )
        # copy plugins
        for framework in frameworks:
            for plugin in MacOSBundle.pluginsMapping[ framework ]:
                pluginsSource = self.qt.pluginsFilePath( plugin, 'macos' )
                if os.path.exists( pluginsSource ):
                    for file in os.listdir( pluginsSource ):
                        # handle debug / release plugins according to framework name
                        if ( '_debug' in file and '_debug' in framework ) or ( not '_debug' in file and not '_debug' in framework ):
                            file = '%s/%s' % ( pluginsSource, file )
                            if not Tools.copy( file, self.qtPluginsFilePath( plugin ) ):
                                return False
        # copy plugins frameworks
        for framework in Tools.getFilesList( self.qtPluginsFilePath(), recursive = True ):
            if not self.copyDependencies( framework, frameworkFilePath ):
                return False
        return True
    
    def deploy(self):
        frameworkFilePath = self.frameworksFilePath()
        # copy binary dependencies
        if not self.copyDependencies( self.binaryFilePath(), frameworkFilePath ):
            return False
        # copy frameworks dependencies
        for framework in Tools.getFilesList( frameworkFilePath ):
            if not self.copyDependencies( framework, frameworkFilePath ):
                return False
        # copy plugins
        return self.deployPlugins()
