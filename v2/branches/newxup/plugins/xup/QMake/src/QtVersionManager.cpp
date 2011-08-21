#include "QtVersionManager.h"

#include <MonkeyCore.h>
#include <MkSShellInterpreter.h>

#include <QProcess>
#include <QDir>
#include <QDebug>

const QString QtVersionManager::mQtVersionKey = "Versions";
const QString QtVersionManager::mQtModuleKey = "Modules";
const QString QtVersionManager::mQtConfigurationKey = "Configurations";
const QRegExp QtVersionManager::mQtVersionRegExp( "4\\.[\\d\\w-_\\.]+" );
const QRegExp QtVersionManager::mQtQMakeRegExp( QString( "QMake version (?:[\\d\\w-_\\.]+)(?:\\r|\\n|\\r\\n)Using Qt version (%1) in (.*)" ).arg( QtVersionManager::mQtVersionRegExp.pattern() ) );
const QRegExp QtVersionManager::mQtUninstallRegExp( "Qt (?:OpenSource|SDK|Commercial) .*" );

QtVersionManager::QtVersionManager( QObject* owner )
	: pSettings( owner, "QtVersions", "1.0.0" )
{
	synchronizeVersions();
	initializeInterpreterCommands( true );
}

QtVersionManager::~QtVersionManager()
{
	initializeInterpreterCommands( false );
}

QtVersionList QtVersionManager::versions() const
{
	QtVersionManager* _this = const_cast<QtVersionManager*>( this );
	QtVersionList items;
	const int count = _this->beginReadArray( mQtVersionKey );

	for ( int i = 0; i < count; i++ )
	{
		_this->setArrayIndex( i );

		items << QtVersion( value( "Version" ).toString(),
							value( "Path" ).toString(),
							value( "Default" ).toBool(),
							value( "QMakeSpec" ).toString(),
							value( "QMakeParameters" ).toString(),
							value( "HasQt4Suffixe" ).toBool() );
	}

	_this->endArray();
	return items;
}

void QtVersionManager::setVersions( const QtVersionList& versions )
{
	beginWriteArray( mQtVersionKey );

	for ( int i = 0; i < versions.count(); i++ )
	{
		setArrayIndex( i );
		const QtVersion& version = versions.at( i );

		setValue( "Version", version.Version );
		setValue( "Path", version.Path );
		setValue( "Default", version.Default );
		setValue( "QMakeSpec", version.QMakeSpec );
		setValue( "QMakeParameters", version.QMakeParameters );
		setValue( "HasQt4Suffixe", version.HasQt4Suffix );
	}

	endArray();
}

QtVersion QtVersionManager::defaultVersion() const
{
	const QtVersionList versions = this->versions();

	foreach ( const QtVersion& version, versions )
	{
		if ( version.Default )
		{
			return version;
		}
	}

	return versions.value( 0 );
}

QtVersion QtVersionManager::version( const QString& versionString ) const
{
	foreach ( const QtVersion& version, versions() )
	{
		if ( version.Version == versionString )
		{
			return version;
		}
	}

	return defaultVersion();
}

// Last Qt version check for modules / configurations: 4.7.2

QtItemList QtVersionManager::defaultModules() const
{
	return QtItemList()
		<< QtItem( "Software Development", QString::null, QString::null, "Modules for general software development" )
		<< QtItem( "QtCore", "core", "QT", "Core non-graphical classes used by other modules" )
		<< QtItem( "QtGui", "gui", "QT", "Graphical user interface (GUI) components" )
		<< QtItem( "QtMultimedia", "multimedia", "QT", "Classes for low-level multimedia functionality" )
		<< QtItem( "QtNetwork", "network", "QT", "Classes for network programming" )
		<< QtItem( "QtOpenGL", "opengl", "QT", "OpenGL support classes" )
		<< QtItem( "QtOpenVG", " ", " ", "OpenVG support classes" )
		<< QtItem( "QtScript", "script", "QT", "Classes for evaluating Qt Scripts" )
		<< QtItem( "QtScriptTools", "scripttools", "QT", "Additional Qt Script components" )
		<< QtItem( "QtSql", "sql", "QT", "Classes for database integration using SQL" )
		<< QtItem( "QtSvg", "svg", "QT", "Classes for displaying the contents of SVG files" )
		<< QtItem( "QtWebKit", "webkit", "QT", "Classes for displaying and editing Web content" )
		<< QtItem( "QtXml", "xml", "QT", "Classes for handling XML" )
		<< QtItem( "QtXmlPatterns", "xmlpatterns", "QT", "An XQuery & XPath engine for XML and custom data models" )
		<< QtItem( "QtDeclarative", "declarative", "QT", "An engine for declaratively building fluid user interfaces" )
		<< QtItem( "Phonon", "phonon", "QT", "Multimedia framework classes" )
		<< QtItem( "Qt3Support", "qt3support", "QT", "Qt 3 compatibility classes" )
		
		<< QtItem( "Qt's Tools", QString::null, QString::null, "Modules for working with Qt's tools" )
		<< QtItem( "QtDesigner", "designer", "CONFIG", "Classes for extending Qt Designer" )
		<< QtItem( "QtUiTools", "uitools", "CONFIG", "Classes for handling Qt Designer forms in applications" )
		<< QtItem( "QtHelp", "help", "CONFIG", "Classes for online help" )
		<< QtItem( "QtAssistant", "assistant", "CONFIG", "Add support for Qt Assistant classes" )
		<< QtItem( "QtTest", "qtestlib", "CONFIG", "Tool classes for unit testing" )
		
		<< QtItem( "Windows", QString::null, QString::null, "Modules for Windows developers" )
		<< QtItem( "QAxContainer", "qaxcontainer", "CONFIG", "Extension for accessing ActiveX controls" )
		<< QtItem( "QAxServer", "qaxserver", "CONFIG", "Extension for writing ActiveX servers" )
		
		<< QtItem( "Unix", QString::null, QString::null, "Modules for Unix developers" )
		<< QtItem( "QtDBus", "dbus", "QT", "Classes for Inter-Process Communication using the D-Bus" )
		;
}

QtItemList QtVersionManager::modules() const
{
	QtVersionManager* _this = const_cast<QtVersionManager*>( this );
	QtItemList modules = defaultModules();
	const int count = _this->beginReadArray( mQtModuleKey );

	for ( int i = 0; i < count; i++ )
	{
		_this->setArrayIndex( i );

		const QtItem module( value( "Text" ).toString(),
						value( "Value" ).toString(),
						value( "Variable" ).toString(),
						value( "Help" ).toString() );

		if ( !modules.contains( module ) )
		{
			modules << module;
		}
	}

	_this->endArray();
	return modules;
}

void QtVersionManager::setModules( const QtItemList& modules )
{
	beginWriteArray( mQtModuleKey );

	for ( int i = 0; i < modules.count(); i++ )
	{
		setArrayIndex( i );
		const QtItem& module = modules.at( i );

		setValue( "Text", module.Text );
		setValue( "Value", module.Value );
		setValue( "Variable", module.Variable );
		setValue( "Help", module.Help );
	}

	endArray();
}

QtItemList QtVersionManager::defaultConfigurations() const
{
	return QtItemList()
		<< QtItem( "Build", QString::null, QString::null, "Build configuration options" )
		<< QtItem( "release", "release", "CONFIG", "The project is to be built in release mode. This is ignored if debug is also specified" )
		<< QtItem( "debug",	"debug", "CONFIG", "The project is to be built in debug mode" )
		<< QtItem( "debug_and_release",	"debug_and_release", "CONFIG", "The project is built in both debug and release modes" )
		<< QtItem( "debug_and_release_target",	"debug_and_release_target", "CONFIG", "The project is built in both debug and release modes. TARGET is built into both the debug and release directories" )
		<< QtItem( "build_all",	"build_all", "CONFIG", "If debug_and_release is specified, the project is built in both debug and release modes by default" )
		<< QtItem( "ordered", "ordered", "CONFIG", "When using the subdirs template, this option specifies that the directories listed should be processed in the order in which they are given" )
		
		<< QtItem( "Compiler/Linker", QString::null, QString::null, "Compiler/Linker configuration" )
		<< QtItem( "warn_on", "warn_on", "CONFIG", "The compiler should output as many warnings as possible. This is ignored if warn_off is specified" )
		<< QtItem( "warn_off", "warn_off", "CONFIG", "The compiler should output as few warnings as possible" )
		<< QtItem( "thread", "thread", "CONFIG", "The target is a multi-threaded application or library. The proper defines and compiler flags will automatically be added to the project" )
		<< QtItem( "x11", "x11", "CONFIG", "The target is a X11 application or library. The proper include paths and libraries will automatically be added to the project" )
		<< QtItem( "windows", "windows", "CONFIG", "The target is a Win32 window application (app only). The proper include paths, compiler flags and libraries will automatically be added to the project" )
		<< QtItem( "console", "console", "CONFIG", "The target is a Win32 console application (app only). The proper include paths, compiler flags and libraries will automatically be added to the project" )
		<< QtItem( "exceptions", "exceptions", "CONFIG", "Exception support is enabled" )
		<< QtItem( "rtti", "rtti", "CONFIG", "RTTI support is enabled" )
		<< QtItem( "stl", "stl", "CONFIG", "STL support is enabled" )
		<< QtItem( "autogen_precompile_source", "autogen_precompile_source", "CONFIG", "Automatically generates a .cpp file that includes the precompiled header file specified in the .pro file" )
		<< QtItem( "precompile_header", "precompile_header", "CONFIG", "Enables support for the use of precompiled headers in projects" )
		<< QtItem( "mmx", "mmx", "CONFIG", "Intel MMX instruction support is enabled" )
		<< QtItem( "3dnow", "3dnow", "CONFIG", "AMD 3DNow! instruction support is enabled" )
		<< QtItem( "sse", "sse", "CONFIG", "SSE support is enabled" )
		<< QtItem( "sse2", "sse2", "CONFIG", "SSE2 support is enabled" )
		
		<< QtItem( "Linker", QString::null, QString::null, "Linker configuration" )
		<< QtItem( "no_lflags_merge", "no_lflags_merge", "CONFIG", "Ensures that the list of libraries stored in the LIBS variable is not reduced to a list of unique values before it is used" )
		
		<< QtItem( "Qt", QString::null, QString::null, "Qt configuration" )
		<< QtItem( "qt", "qt", "CONFIG", "The target is a Qt application/library and requires the Qt library and header files. The proper include and library paths for the Qt library will automatically be added to the project. This is defined by default, and can be fine-tuned with the QT variable" )
		<< QtItem( "uic3", "uic3", "CONFIG", "Configures qmake to run uic3 on the content of FORMS3 if defined; otherwise the contents of FORMS will be processed instead" )
		<< QtItem( "resources", "resources", "CONFIG", "Configures qmake to run rcc on the content of RESOURCES if defined" )
		
		<< QtItem( "Others", QString::null, QString::null, "Others configuration" )
		<< QtItem( "create_prl", "create_prl", "CONFIG", "This option enables qmake to track these dependencies. When this option is enabled, qmake will create a file ending in .prl which will save meta-information about the library (see Library Dependencies for more info)" )
		<< QtItem( "link_prl", "", "CONFIG", "When this is enabled, qmake will process all libraries linked to by the application and find their meta-information (see Library Dependencies for more info)" )
		<< QtItem( "copy_dir_files", "copy_dir_files", "CONFIG", "Enables the install rule to also copy directories, not just files" )
		
		<< QtItem( "Windows", QString::null, QString::null, "Windows specific configuration" )
		<< QtItem( "flat", "flat", "CONFIG", "When using the vcapp template this will put all the source files into the source group and the header files into the header group regardless of what directory they reside in. Turning this option off will group the files within the source/header group depending on the directory they reside. This is turned on by default" )
		<< QtItem( "embed_manifest_dll", "embed_manifest_dll", "CONFIG", "Embeds a manifest file in the DLL created as part of a library project" )
		<< QtItem( "embed_manifest_exe", "embed_manifest_exe", "CONFIG", "Embeds a manifest file in the DLL created as part of an application project" )
		<< QtItem( "incremental", "incremental", "CONFIG", "Used to enable or disable incremental linking in Visual C++, depending on whether this feature is enabled or disabled by default" )
		
		<< QtItem( "Mac", QString::null, QString::null, "Mac OS X specific configuration" )
		<< QtItem( "ppc", "ppc", "CONFIG", "Builds a PowerPC binary" )
		<< QtItem( "x86", "x86", "CONFIG", "Builds an i386 compatible binary" )
		<< QtItem( "app_bundle", "app_bundle", "CONFIG", "Puts the executable into a bundle (this is the default)" )
		<< QtItem( "lib_bundle", "lib_bundle", "CONFIG", "Puts the library into a library bundle" )
		
		<< QtItem( "Unix", QString::null, QString::null, "Unix specific configuration" )
		<< QtItem( "largefile", "largefile", "CONFIG", "Includes support for large files" )
		<< QtItem( "separate_debug_info", "separate_debug_info", "CONFIG", "Puts debugging information for libraries in separate files" )
		
		<< QtItem( "Symbian", QString::null, QString::null, "Symbian specific configuration" )
		<< QtItem( "stdbinary", "stdbinary", "CONFIG", "Builds an Open C binary (i.e. STDDLL, STDEXE, or STDLIB, depending on the target binary type.)" )
		<< QtItem( "no_icon", "no_icon", "CONFIG", "Doesn't generate resources needed for displaying an icon for executable in application menu (app only)" )
		<< QtItem( "symbian_test", "symbian_test", "CONFIG", "Places mmp files and extension makefiles under test sections in generated bld.inf instead of their regular sections. Note that this only affects automatically generated bld.inf content; the content added via BLD_INF_RULES variable is not affected" )
		<< QtItem( "localize_deployment", "localize_deployment", "CONFIG", "Makes lupdate tool add fields for application captions and package file names into generated .ts files. Qmake generates properly localized .loc and .pkg files based on available translations. Translation file name bodies must end with underscore and the language code for deployment localization to work. E.g. myapp_en.ts. Note: All languages supported by Qt are not supported by Symbian, so some .ts files may be ignored by qmake" )
		
		/*<< QtItem( "rtti", "rtti", "CONFIG", "RTTI support is enabled." )
		<< QtItem( "stl", "stl", "CONFIG", "STL support is enabled." )
		<< QtItem( "exceptions", "exceptions", "CONFIG", "Exception support is enabled." )
		<< QtItem( "thread", "thread", "CONFIG", "The target is a multi-threaded application or library. The proper defines and compiler flags will automatically be added to the project." )
		<< QtItem( "no_lflags_merge", "no_lflags_merge", "CONFIG", "Ensures that the list of libraries stored in the LIBS variable is not reduced to a list of unique values before it is used." )
		<< QtItem( "LIB ONLY", QString::null, QString::null, "Options for LIB template only" )
		<< QtItem( "dll", "dll", "CONFIG", "The target is a shared object/DLL.The proper include paths, compiler flags and libraries will automatically be added to the project." )
		<< QtItem( "staticlib", "staticlib", "CONFIG", "The target is a static library (lib only). The proper compiler flags will automatically be added to the project." )
		<< QtItem( "plugin", "plugin", "CONFIG", "The target is a plugin (lib only). This enables dll as well." )
		<< QtItem( "X11 ONLY", QString::null, QString::null, "Options for X11 only" )
		<< QtItem( "x11", "x11", "CONFIG", "The target is a X11 application or library. The proper include paths and libraries will automatically be added to the project." )
		<< QtItem( "MAC OS X ONLY", QString::null, QString::null, "Options for Mac OS X only" )
		<< QtItem( "ppc", "ppc", "CONFIG", "Builds a PowerPC binary." )
		<< QtItem( "ppc64", "ppc64", "CONFIG", "Builds a 64 bits PowerPC binary." )
		<< QtItem( "x86", "x86", "CONFIG", "Builds an i386 compatible binary." )
		<< QtItem( "x86_64", "x86_64", "CONFIG", "Builds an x86 64 bits compatible binary." )
		<< QtItem( "app_bundle", "app_bundle", "CONFIG", "Puts the executable into a bundle (this is the default)." )
		<< QtItem( "lib_bundle", "lib_bundle", "CONFIG", "Puts the library into a library bundle." )
		<< QtItem( "WINDOWS ONLY", QString::null, QString::null, "Options for Windows only" )
		<< QtItem( "windows", "windows", "CONFIG", "The target is a Win32 window application (app only). The proper include paths,compiler flags and libraries will automatically be added to the project." )
		<< QtItem( "console", "console", "CONFIG", "The target is a Win32 console application (app only). The proper include paths, compiler flags and libraries will automatically be added to the project." )
		<< QtItem( "flat", "flat", "CONFIG", "When using the vcapp template this will put all the source files into the source group and the header files into the header group regardless of what directory they reside in. Turning this option off will group the files within the source/header group depending on the directory they reside. This is turned on by default." )
		<< QtItem( "embed_manifest_exe", "embed_manifest_dll", "CONFIG", "Embeds a manifest file in the DLL created as part of an application project." )
		<< QtItem( "embed_manifest_dll", "embed_manifest_dll", "CONFIG", "Embeds a manifest file in the DLL created as part of an library project." )
		<< QtItem( "ACTIVEQT ONLY", QString::null, QString::null, "Option for Windows/Active Qt only" )
		<< QtItem( "qaxserver_no_postlink", "qaxserver_no_postlink", "CONFIG", "No help available" )
		<< QtItem( "Qt ONLY", QString::null, QString::null, "Options for Qt only" )
		<< QtItem( "ordered", "ordered", "CONFIG", "Sub project are built in defined order." )
		<< QtItem( "qt", "qt", "CONFIG", "The target is a Qt application/library and requires the Qt library and header files. The proper include and library paths for the Qt library will automatically be added to the project. This is defined by default, and can be fine-tuned with the QT variable." )
		<< QtItem( "resources", "resources", "CONFIG", "Configures qmake to run rcc on the content of RESOURCES if defined." )
		<< QtItem( "uic3", "uic3", "CONFIG", "Configures qmake to run uic3 on the content of FORMS3 if defined; otherwise the contents of FORMS will be processed instead." )*/
		;
}

QtItemList QtVersionManager::configurations() const
{
	QtVersionManager* _this = const_cast<QtVersionManager*>( this );
	QtItemList configurations = defaultConfigurations();
	const int count = _this->beginReadArray( mQtConfigurationKey );

	for ( int i = 0; i < count; i++ )
	{
		_this->setArrayIndex( i );

		const QtItem configuration( value( "Text" ).toString(),
									value( "Value" ).toString(),
									value( "Variable" ).toString(),
									value( "Help" ).toString() );

		if ( !configurations.contains( configuration ) )
		{
			configurations << configuration;
		}
	}

	_this->endArray();
	return configurations;
}

void QtVersionManager::setConfigurations( const QtItemList& configurations )
{
	beginWriteArray( mQtConfigurationKey );

	for ( int i = 0; i < configurations.count(); i++ )
	{
		setArrayIndex( i );
		const QtItem& configuration = configurations.at( i );

		setValue( "Text", configuration.Text );
		setValue( "Value", configuration.Value );
		setValue( "Variable", configuration.Variable );
		setValue( "Help", configuration.Help );
	}

	endArray();
}

#if defined( Q_OS_WIN )
QStringList QtVersionManager::possibleQtPaths() const
{
	QSet<QString> paths;

	paths << QString::null; // for qmake available in PATH

	// get users uninstall
	QSettings* settings = new QSettings( QSettings::UserScope, "Microsoft", "Windows" );
	settings->beginGroup( "CurrentVersion/Uninstall" );

	foreach ( const QString& key, settings->childGroups() )
	{
		QString path = settings->value( QString( "%1/MINGW_INSTDIR" ).arg( key ) ).toString();

		if ( path.isEmpty() )
		{
			path = settings->value( QString( "%1/QTSDK_INSTDIR" ).arg( key ) ).toString();

			if ( !path.isEmpty() )
			{
				path.append( "/qt" );
			}
		}

		if ( !path.isEmpty() )
		{
			paths << path;
		}
	}

	// get system uninstalls
	delete settings;
	settings = new QSettings( QSettings::SystemScope, "Microsoft", "Windows" );
	settings->beginGroup( "CurrentVersion/Uninstall" );

	foreach ( const QString& key, settings->childGroups() )
	{
		QString path = settings->value( QString( "%1/MINGW_INSTDIR" ).arg( key ) ).toString();

		if ( path.isEmpty() )
		{
			path = settings->value( QString( "%1/QTSDK_INSTDIR" ).arg( key ) ).toString();

			if ( !path.isEmpty() )
			{
				path.append( "/qt" );
			}
		}

		if ( !path.isEmpty() && !paths.contains( path ) )
		{
			paths << path;
		}
	}

	delete settings;
	return paths.toList();
}
#elif defined( Q_OS_MAC )
QStringList QtVersionManager::possibleQtPaths() const
{
	return QStringList() << QString::null; // for qmake available in PATH
}
#else
QStringList QtVersionManager::possibleQtPaths() const
{
	return QStringList() << QString::null; // for qmake available in PATH
}
#endif

QtVersionList QtVersionManager::getQtVersions( const QStringList& paths ) const
{
	QtVersionList versions;
	bool hasDefaultVersion = defaultVersion().isValid();

	foreach ( const QString& path, paths )
	{
		QtVersion sysQt;
		QProcess process;
		QString datas;
		bool hasSuffix = true;
		QString prefix = path.isEmpty() ? QString::null : path +"/bin/";

		// trying with -qt4 suffix first
		process.start( QString( "\"%1qmake-qt4\" -v" ).arg( prefix ) );
		process.waitForFinished();
		datas = QString::fromLocal8Bit( process.readAll() ).trimmed();

		// else without suffix
		if ( !mQtQMakeRegExp.exactMatch( datas ) )
		{
			process.start( QString( "\"%1qmake\" -v" ).arg( prefix ) );
			process.waitForFinished();
			datas = QString::fromLocal8Bit( process.readAll() ).trimmed();
			hasSuffix = false;
		}

		// if matching output add version
		if ( mQtQMakeRegExp.exactMatch( datas ) )
		{
			const QString version = mQtQMakeRegExp.cap( 1 );
			const QString path = QDir::toNativeSeparators( mQtQMakeRegExp.cap( 2 ).replace( "\\", "/" ).section( '/', 0, -2 ) );

			sysQt.Version = QString( "Qt System (%1)" ).arg( version );
			sysQt.Path = path;
			sysQt.Default = hasDefaultVersion ? false : true;
			sysQt.QMakeSpec = QString::null;
			sysQt.QMakeParameters = "\"$cp$\"";
			sysQt.HasQt4Suffix = hasSuffix;

			if ( !hasDefaultVersion )
			{
				hasDefaultVersion = true;
			}

			versions << sysQt;
		}
	}

	return versions;
}

void QtVersionManager::synchronizeVersions()
{
	const QStringList paths = possibleQtPaths();
	const QtVersionList newVersions = getQtVersions( paths );
	const QtVersionList versions = this->versions();
	QMap<quint32, QtVersion> items;

	// add existing items
	for ( int i = 0; i < versions.count(); i++ )
	{
		const QtVersion& version = versions.at( i );

		items[ version.hash() ] = version;
	}

	// add new ones if needed
	foreach ( const QtVersion& newVersion, newVersions )
	{
		if ( items.contains( newVersion.hash() ) )
		{
			const QtVersion& v = items[ newVersion.hash() ];

			if ( v.Version == newVersion.Version )
			{
				continue;
			}
		}

		items[ newVersion.hash() ] = newVersion;
	}

	// update qt versions
	setVersions( items.values() );
}

void QtVersionManager::initializeInterpreterCommands( bool initialize )
{
	if ( initialize ) {
		// register command
		QString help = MkSShellInterpreter::tr(
			"This command manage the Qt versions, usage:\n"
			"\tqtversion xml [version]"
		);

		MonkeyCore::interpreter()->addCommandImplementation( "qtversion", QtVersionManager::commandInterpreter, help, this );
	}
	else {
		MonkeyCore::interpreter()->removeCommandImplementation( "qtversion" );
	}
}

QString QtVersionManager::commandInterpreter( const QString& command, const QStringList& _arguments, int* result, MkSShellInterpreter* interpreter, void* data )
{
	Q_UNUSED( command );
	Q_UNUSED( interpreter );
	QtVersionManager* manager = static_cast<QtVersionManager*>( data );
	QStringList arguments = _arguments;
	const QStringList allowedOperations = QStringList( "xml" );

	if ( result ) {
		*result = MkSShellInterpreter::NoError;
	}

	if ( arguments.isEmpty() ) {
		if ( result ) {
			*result = MkSShellInterpreter::InvalidCommand;
		}

		return MkSShellInterpreter::tr( "Operation not defined. Available operations are: %1." ).arg( allowedOperations.join( ", " ) );
	}

	const QString operation = arguments.takeFirst();

	if ( !allowedOperations.contains( operation ) ) {
		if ( result ) {
			*result = MkSShellInterpreter::InvalidCommand;
		}

		return MkSShellInterpreter::tr( "Unknown operation: '%1'." ).arg( operation );
	}

	if ( operation == "xml" ) {
		if ( arguments.count() != 1 ) {
			if ( result ) {
				*result = MkSShellInterpreter::InvalidCommand;
			}

			return MkSShellInterpreter::tr( "'set' operation take 1 argument, %1 given." ).arg( arguments.count() );
		}

		const QString version = arguments.at( 0 );

		return manager->version( version ).toXml();
	}

	return QString::null;
}

