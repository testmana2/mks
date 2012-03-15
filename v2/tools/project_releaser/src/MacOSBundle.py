import os

import Project
import Tools
import Qt
import Git
import Svn
import Wine

class ProfPrinter:
    prof = 0;
    
    def plus(self):
        self.prof += 1
    
    def minus(self):
        self.prof -= 1
    
    def show(self, string, indent = '  '):
        print ( indent *self.prof ) +'- ' +string

class MacOSBundle:
    prof = ProfPrinter()
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
        ok, output = Tools.executeAndGetOutput( command, showExecInfo = False )
        items = []
        if ok:
            for line in set( output.splitlines() ):
                line = line.strip();
                tmp = line.lower()
                # system frameworks are skipped
                if tmp.startswith( '/usr/lib' ) or tmp.startswith( '/system/library' ):
                    continue
                index = line.rfind( '(compatibility' )
                filePath = line[ :index ].strip()
                # make dependency only if file base name are not equals
                if binaryFilePath != os.path.basename( filePath ):
                    items.append( filePath )
        #print items
        #print output
        return items
    
    def relinkDependency(self, binaryFilePath, sourceFilePath, targetFilePath):
        self.prof.plus()
        self.prof.show( 'Set id for %s' % ( targetFilePath ) )
        self.prof.show( 'Set change for %s' % ( binaryFilePath ) )
        installTool = self.bin( 'install_name_tool' )
        frameworkName = os.path.basename( sourceFilePath )
        idCommand = '"%s" -id "@executable_path/../Frameworks/%s" "%s"' % ( installTool, frameworkName, targetFilePath )
        changeCommand = '"%s" -change "%s" "@executable_path/../Frameworks/%s" "%s"' % ( installTool, sourceFilePath, frameworkName, targetFilePath )
        Tools.execute( idCommand, showError = False, showExecInfo = False )
        Tools.execute( changeCommand, showError = False, showExecInfo = False )
        self.prof.minus()
        return True
    
    def copyDependency(self, binaryFilePath, sourceFilePath, targetPath):
        targetFilePath = '%s/%s' % ( targetPath, os.path.basename( sourceFilePath[ 0 ] ) )
        self.prof.plus()
        self.prof.show( 'Copying dependency %s' % ( sourceFilePath[ 0 ] ) )
        if not os.path.exists( targetFilePath ) and not Tools.copy( sourceFilePath[ 0 ], targetPath ):
            self.prof.minus()
            return False
        result = self.relinkDependency( binaryFilePath, sourceFilePath[ 1 ], targetFilePath );
        self.prof.minus()
        return result
    
    def copyDependencies(self, binaryFilePath, frameworksPath):
        if not Tools.createDirectory( frameworksPath ):
            return False
        self.prof.plus()
        self.prof.show( 'Copying dependencies for %s' % ( binaryFilePath ) )
        frameworks = self.getDependencies( binaryFilePath )
        if not frameworks:
            self.prof.plus()
            self.prof.show( '(no dependencies)' )
            self.prof.minus()
        # copy frameworks
        for framework in frameworks:
            localFramework = framework
            # rebuilt framework path if needed (crossbuild)
            if not os.path.exists( framework ) and ( 'Qt' in framework or 'phonon' in framework ):
                frameworkName = os.path.basename( framework )
                index = framework.find( '.framework' )
                localFramework = '%s/lib/%s%s' % ( self.qt.qt.macos, frameworkName, framework[ index: ] )
            if not os.path.exists( framework ) and ( not 'Qt' in framework and not 'phonon' in framework ):
                self.prof.plus()
                self.prof.show( 'Skip non existing dependency %s' % ( framework ) )
                self.prof.minus()
                continue
            # copy dependency
            if not self.copyDependency( binaryFilePath, [ localFramework, framework ], frameworksPath ):
                self.prof.minus()
                return False
            self.prof.plus()
            # copy dependencies of the dependency
            targetFilePath = '%s/%s' % ( frameworksPath, os.path.basename( localFramework ) )
            if not self.copyDependencies( targetFilePath, frameworksPath ):
                self.prof.minus()
                self.prof.minus()
                return False
            self.prof.minus()
        self.prof.minus()
        return True
    
    def deployPlugins(self):
        frameworksName = os.listdir( self.frameworksFilePath() )
        # copy frameworks plugins
        for frameworkName in frameworksName:
            for plugin in MacOSBundle.pluginsMapping[ frameworkName ]:
                pluginsPath = self.qt.pluginsFilePath( plugin, 'macos' )
                if os.path.exists( pluginsPath ):
                    for file in os.listdir( pluginsPath ):
                        # handle debug / release plugins according to framework name
                        if ( '_debug' in file and '_debug' in frameworkName ) or ( not '_debug' in file and not '_debug' in frameworkName ):
                            file = '%s/%s' % ( pluginsPath, file )
                            if not Tools.copy( file, self.qtPluginsFilePath( plugin ) ):
                                return False
        # copy plugins frameworks
        for frameworkFilePath in Tools.getFilesList( self.qtPluginsFilePath(), recursive = True ):
            if not self.copyDependencies( frameworkFilePath, self.frameworksFilePath() ):
                return False
        return True
    
    def deploy(self):
        # copy binary dependencies
        if not self.copyDependencies( self.binaryFilePath(), self.frameworksFilePath() ):
            return False
        # copy plugins
        return self.deployPlugins()
        return True
