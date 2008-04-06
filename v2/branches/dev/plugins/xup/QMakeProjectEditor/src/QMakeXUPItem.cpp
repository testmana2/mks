#include "QMakeXUPItem.h"
#include "QMake2XUP.h"
#include "QtVersionManager.h"

#include <QFileInfo>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

#include <monkey.h>
#include <pluginsmanager.h>

#include <QDebug>

XUPItemInfos QMakeXUPItem::mQMakeXUPItemInfos;

QMakeXUPItem::QMakeXUPItem( const QDomElement& e, const QString& s, bool b )
	: XUPItem()
{
	if ( !mQMakeXUPItemInfos.Registered )
		registerItem();
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

void QMakeXUPItem::registerItem()
{
	mQMakeXUPItemInfos.Registered = true;
	// registering...
	mQMakeXUPItemInfos.TextTypes = XUPItem::textTypes();
	mQMakeXUPItemInfos.Operators = XUPItem::operators();
	// filtered variables
	mQMakeXUPItemInfos.FilteredVariables
		<< "SUBDIRS"
		<< "FORMS"
		<< "FORMS3"
		<< "HEADERS"
		<< "SOURCES"
		<< "OBJECTIVE_SOURCES"
		<< "TRANSLATIONS"
		<< "RESOURCES"
		<< "RC_FILE"
		<< "RES_FILE"
		<< "DEF_FILE"
		<< "INCLUDEPATH"
		<< "DEPENDPATH"
		<< "VPATH"
		<< "LIBS"
		<< "DEFINES"
		<< "OTHER_FILES";
	// variables based on files
	mQMakeXUPItemInfos.FileVariables
		<< "FORMS"
		<< "FORMS3"
		<< "HEADERS"
		<< "SOURCES"
		<< "OBJECTIVE_SOURCES"
		<< "TRANSLATIONS"
		<< "RESOURCES"
		<< "RC_FILE"
		<< "RES_FILE"
		<< "DEF_FILE"
		<< "SUBDIRS"
		<< "OTHER_FILES";
	// variables based on paths
	mQMakeXUPItemInfos.PathVariables
		<< "INCLUDEPATH"
		<< "DEPENDPATH"
		<< "VPATH";
	// variables labels
	mQMakeXUPItemInfos.VariableLabels[ "FORMS" ] = tr( "Forms Files" );
	mQMakeXUPItemInfos.VariableLabels[ "FORMS3" ] = tr( "Forms 3 Files" );
	mQMakeXUPItemInfos.VariableLabels[ "HEADERS" ] = tr( "Headers Files" );
	mQMakeXUPItemInfos.VariableLabels[ "SOURCES" ] = tr( "Sources Files" );
	mQMakeXUPItemInfos.VariableLabels[ "OBJECTIVE_SOURCES" ] = tr( "Objective Sources Files" );
	mQMakeXUPItemInfos.VariableLabels[ "TRANSLATIONS" ] = tr( "Qt Translations Files" );
	mQMakeXUPItemInfos.VariableLabels[ "RESOURCES" ] = tr( "Qt Resources Files" );
	mQMakeXUPItemInfos.VariableLabels[ "RC_FILE" ] = tr( "Resources Files" );
	mQMakeXUPItemInfos.VariableLabels[ "RES_FILE" ] = tr( "Compiled Resources Files" );
	mQMakeXUPItemInfos.VariableLabels[ "DEF_FILE" ] = tr( "Definitions Files" );
	mQMakeXUPItemInfos.VariableLabels[ "SUBDIRS" ] = tr( "Sub Projects" );
	mQMakeXUPItemInfos.VariableLabels[ "INCLUDEPATH" ] = tr( "Includes Paths" );
	mQMakeXUPItemInfos.VariableLabels[ "DEPENDPATH" ] = tr( "Depends Paths" );
	mQMakeXUPItemInfos.VariableLabels[ "VPATH" ] = tr( "Virtuals Paths" );
	mQMakeXUPItemInfos.VariableLabels[ "LIBS" ] = tr( "Libraries Files" );
	mQMakeXUPItemInfos.VariableLabels[ "DEFINES" ] = tr( "Defines" );
	mQMakeXUPItemInfos.VariableLabels[ "OTHER_FILES" ] = tr( "Other Files" );
	// variables icons
	mQMakeXUPItemInfos.VariableIcons[ "FORMS" ] = getIcon( value( "icon" ), "forms" );
	mQMakeXUPItemInfos.VariableIcons[ "FORMS3" ] = getIcon( value( "icon" ), "forms3" );
	mQMakeXUPItemInfos.VariableIcons[ "HEADERS" ] = getIcon( value( "icon" ), "headers" );
	mQMakeXUPItemInfos.VariableIcons[ "SOURCES" ] = getIcon( value( "icon" ), "sources" );
	mQMakeXUPItemInfos.VariableIcons[ "OBJECTIVE_SOURCES" ] = getIcon( value( "icon" ), "objective_sources" );
	mQMakeXUPItemInfos.VariableIcons[ "TRANSLATIONS" ] = getIcon( value( "icon" ), "translations" );
	mQMakeXUPItemInfos.VariableIcons[ "RESOURCES" ] = getIcon( value( "icon" ), "resources" );
	mQMakeXUPItemInfos.VariableIcons[ "RC_FILE" ] = getIcon( value( "icon" ), "rc_file" );
	mQMakeXUPItemInfos.VariableIcons[ "RES_FILE" ] = getIcon( value( "icon" ), "res_file" );
	mQMakeXUPItemInfos.VariableIcons[ "DEF_FILE" ] = getIcon( value( "icon" ), "def_file" );
	//mQMakeXUPItemInfos.VariableIcons[ "SUBDIRS" ] = getIcon( value( "icon" ), "project" );
	mQMakeXUPItemInfos.VariableIcons[ "INCLUDEPATH" ] = getIcon( value( "icon" ), "includepath" );
	mQMakeXUPItemInfos.VariableIcons[ "DEPENDPATH" ] = getIcon( value( "icon" ), "dependpath" );
	mQMakeXUPItemInfos.VariableIcons[ "VPATH" ] = getIcon( value( "icon" ), "vpath" );
	mQMakeXUPItemInfos.VariableIcons[ "LIBS" ] = getIcon( value( "icon" ), "libs" );
	mQMakeXUPItemInfos.VariableIcons[ "DEFINES" ] = getIcon( value( "icon" ), "defines" );
	mQMakeXUPItemInfos.VariableIcons[ "OTHER_FILES" ] = getIcon( value( "icon" ), "other_files" );
	// register project suffixes
	mQMakeXUPItemInfos.Suffixes[tr( "Qt Project" )] = QStringList( "*.pro" );
	mQMakeXUPItemInfos.Suffixes[tr( "Qt Project Include" )] = QStringList( "*.pri" );
	// register variable suffixes
	// C++ filters
	const QStringList cf = pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
	// HEADERS filters
	QStringList hf;
	foreach ( QString f, cf )
		if ( f.startsWith( "*.h", Qt::CaseInsensitive ) )
			hf << f;
	// SOURCES filters
	QStringList sf;
	foreach ( QString f, cf )
		if ( f.startsWith( "*.c", Qt::CaseInsensitive ) )
			sf << f;
	// YACC filters
	QStringList yf;
	foreach ( QString s, sf )
		if ( !yf.contains( s.replace( "c", "y", Qt::CaseInsensitive ) ) )
			yf << s;
	// LEX filters
	QStringList lf;
	foreach ( QString s, sf )
		if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !lf.contains( s.replace( "c", "l", Qt::CaseInsensitive ) ) )
			yf << s;
	// IMAGES filters
	const QStringList pf = pMonkeyStudio::availableImageFormats();
	// PROJECT filters
	QStringList pjf;
	foreach ( QStringList l, mQMakeXUPItemInfos.Suffixes.values() )
		pjf << l;
	// registering...
	mQMakeXUPItemInfos.VariableSuffixes[ "HEADERS" ] = hf;
	mQMakeXUPItemInfos.VariableSuffixes[ "SOURCES" ] = sf;
	mQMakeXUPItemInfos.VariableSuffixes[ "OBJECTIVE_SOURCES" ] = QStringList( "*.m" ) << "*.mm";
	mQMakeXUPItemInfos.VariableSuffixes[ "LEXSOURCES" ] = lf;
	mQMakeXUPItemInfos.VariableSuffixes[ "YACCSOURCES" ] = yf;
	mQMakeXUPItemInfos.VariableSuffixes[ "IMAGES" ] = pf;
	mQMakeXUPItemInfos.VariableSuffixes[ "FORMS" ] = QStringList( "*.ui" );
	mQMakeXUPItemInfos.VariableSuffixes[ "FORMS3" ] = QStringList( "*.ui" );
	mQMakeXUPItemInfos.VariableSuffixes[ "TRANSLATIONS" ] = QStringList( "*.ts" );
	mQMakeXUPItemInfos.VariableSuffixes[ "RESOURCES" ] = QStringList( "*.qrc" );
	mQMakeXUPItemInfos.VariableSuffixes[ "DEF_FILE" ] = QStringList( "*.def" );
	mQMakeXUPItemInfos.VariableSuffixes[ "RC_FILE" ] = QStringList( "*.rc" );
	mQMakeXUPItemInfos.VariableSuffixes[ "RES_FILE" ] = QStringList( "*.res" );
}

QStringList QMakeXUPItem::operators() const
{ return mQMakeXUPItemInfos.Operators; }

void QMakeXUPItem::registerOperator( const QString& s )
{
	if ( !mQMakeXUPItemInfos.Operators.contains( s ) )
		mQMakeXUPItemInfos.Operators << s;
}

QStringList QMakeXUPItem::filteredVariables() const
{ return mQMakeXUPItemInfos.FilteredVariables; }

void QMakeXUPItem::registerFilteredVariables( const QString& s )
{
	if ( !mQMakeXUPItemInfos.FilteredVariables.contains( s ) )
		mQMakeXUPItemInfos.FilteredVariables << s;
}

QStringList QMakeXUPItem::textTypes() const
{ return mQMakeXUPItemInfos.TextTypes; }

void QMakeXUPItem::registerTextType( const QString& s )
{
	if ( !mQMakeXUPItemInfos.TextTypes.contains( s ) )
		mQMakeXUPItemInfos.TextTypes << s;
}

QStringList QMakeXUPItem::fileVariables() const
{ return mQMakeXUPItemInfos.FileVariables; }

void QMakeXUPItem::registerFileVariables( const QString& s )
{
	if ( !mQMakeXUPItemInfos.FileVariables.contains( s ) )
		mQMakeXUPItemInfos.FileVariables << s;
}

QStringList QMakeXUPItem::pathVariables() const
{ return mQMakeXUPItemInfos.PathVariables; }

void QMakeXUPItem::registerPathVariables( const QString& s )
{
	if ( !mQMakeXUPItemInfos.PathVariables.contains( s ) )
		mQMakeXUPItemInfos.PathVariables << s;
}

QHash<QString, QStringList> QMakeXUPItem::suffixes() const
{ return mQMakeXUPItemInfos.Suffixes; }

void QMakeXUPItem::registerSuffixes( const QString& l, const QStringList& s )
{ mQMakeXUPItemInfos.Suffixes[l] = s; }

QHash<QString, QString> QMakeXUPItem::variableLabels() const
{ return mQMakeXUPItemInfos.VariableLabels; }

void QMakeXUPItem::registerVariableLabels( const QString& v, const QString& l )
{ mQMakeXUPItemInfos.VariableLabels[v] = l; }

QHash<QString, QIcon> QMakeXUPItem::variableIcons() const
{ return mQMakeXUPItemInfos.VariableIcons; }

void QMakeXUPItem::registerVariableIcons( const QString& v, const QIcon& i )
{ mQMakeXUPItemInfos.VariableIcons[v] = i; }

QHash<QString, QStringList> QMakeXUPItem::variableSuffixes() const
{ return mQMakeXUPItemInfos.VariableSuffixes; }

void QMakeXUPItem::registerVariableSuffixes( const QString& n, const QStringList& s )
{ mQMakeXUPItemInfos.VariableSuffixes[n] = s; }

QIcon QMakeXUPItem::getIcon( const QString& o, const QString& d ) const
{
	QString s = QFile::exists( o ) ? o : QString( ":/qmakeitems/%1.png" ).arg( d );
	return QFile::exists( s ) ? QIcon( s ) : XUPItem::getIcon( o, d );
}

QMakeXUPItem* QMakeXUPItem::clone( bool b ) const
{ return b ? new QMakeXUPItem( domElement(), projectFilePath(), modified() ) : new QMakeXUPItem; }

void QMakeXUPItem::remove()
{
	// remove project from tree if needed
	if ( isType( "value" ) && parent()->defaultValue() == "SUBDIRS" )
	{
		// get project file
		QFileInfo fi( filePath() );
		if ( fi.isDir() )
		{
			const QString fn = fi.fileName().append( ".pro" );
			fi = QDir( fi.absoluteFilePath() ).entryInfoList( QStringList( fn ), QDir::Files | QDir::Readable, QDir::Name | QDir::IgnoreCase ).value( 0 );
		}
	
		// check direct project to find it
		foreach ( XUPItem* it, project()->children( false, false ) )
		{
			if ( it->isProject() && it->projectFilePath() == fi.absoluteFilePath() )
			{
				// close relating project
				it->closeProject();
				// normal xup remove
				XUPItem::remove();
				return;
			}
		}
	}
	// normal xup remove item
	XUPItem::remove();
}

QString QMakeXUPItem::interpretedVariable( const QString& s, const XUPItem* it, const QString& d ) const
{
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read from environment when qmake run
		$(QTDIR) : read from generated makefile
	*/
	QString dv = QString( s ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	// environment var
	if ( s.startsWith( "$$(" ) || dv == "PWD" )
		return dv != "PWD" ? qgetenv( qPrintable( dv ) ) : projectPath();
	else if ( s.startsWith( "$(" ) )
		qWarning() << "Don't know how to proceed: " << s;
	else if ( s.startsWith( "$$[" ) )
		qWarning() << "Don't know how to proceed: " << s;
	else
	{
		QString v;
		if ( XUPItem* pi = topProjectForInclude() )
		{
			foreach ( XUPItem* cit, pi->children( true, false ) )
			{
				if ( cit->topProjectForInclude() != pi )
					continue;
				if ( it && cit == it )
					return v;
				if ( cit->isType( "variable" ) && cit->defaultValue() == dv )
				{
					if ( cit->hasChildren() )
					{
						const QString o = cit->value( "operator", "=" );
						QString cv;
						foreach ( XUPItem* ccit, cit->children( false, false ) )
							cv += ccit->defaultInterpretedValue() +" ";
						cv = cv.trimmed();
						if ( o == "=" )
							v = cv;
						else if ( o == "-=" )
							v.remove( cv );
						else if ( o == "+=" )
							v.append( " " +cv );
						else if ( o == "*=" )
						{
							if ( !v.contains( cv ) )
								v.append( " " +cv );
						}
						else if ( o == "~=" )
						{
							qWarning() << "Don't know how to interpretate ~= operator";
						}
					}
				}
			}
		}
		return v;
	}
	return XUPItem::interpretedVariable( s, it, d );
}

QString QMakeXUPItem::defaultInterpretedValue() const
{
	QRegExp rx( "\\$\\$?[\\{\\(\\[]?(\\w+)[\\}\\)\\]]?" );
	const QString dv = defaultValue();
	QString v = dv;
	int p = 0;
	while ( ( p = rx.indexIn( dv, p ) ) != -1 )
	{
		v.replace( rx.capturedTexts().value( 0 ), interpretedVariable( rx.capturedTexts().value( 0 ), this ) );
		p += rx.matchedLength();
	}
	return v;
}

void QMakeXUPItem::checkChildrenProjects()
{
	foreach ( XUPItem* it, children( true, true ) )
	{
		QString fn;
		// get subdirs values
		if ( it->isType( "variable" ) && it->defaultValue() == "SUBDIRS" )
		{
			foreach ( XUPItem* cit, it->children( false, false ) )
			{
				// get filename
				fn = cit->filePath();
				QFileInfo fi( fn );
				if ( fi.isDir() )
					fn.append( QString( "/%1.pro" ).arg( fi.fileName() ) );
				// if file exists open project
				if ( QFile::exists( fn ) )
				{
					XUPItem* pi = clone( false );
					if ( pi->loadProject( fn ) )
						it->project()->appendRow( pi );
					else
						delete pi;
				}
			}
		}
		else if ( it->isType( "function" ) && it->defaultValue().startsWith( "include" ) )
		{
			// get filename
			fn = it->defaultInterpretedValue();
			fn.remove( 0, fn.indexOf( '(' ) +1 );
			fn.chop( fn.length() -fn.lastIndexOf( ')' ) );
			fn = it->filePath( fn.trimmed() );
			
			// open project include if it exists
			if ( QFile::exists( fn ) )
			{
				XUPItem* pi = clone( false );
				if ( pi->loadProject( fn ) )
					it->appendRow( pi );
				else
				{
					delete pi;
					qWarning( "QMakeXUPItem: error loading include file: %s", qPrintable( fn ) );
				}
			}
		}
	}
}

bool QMakeXUPItem::isProjectContainer() const
{ return interpretedVariable( "TEMPLATE", this ) == "subdirs"; }

bool QMakeXUPItem::loadProject( const QString& s, const QString& v )
{
	if ( XUPIO::loadXUP( this, QMake2XUP::convertFromPro( s, v ), v ) )
	{
		mProjectFilePath = s;
		setModified( false );
		checkChildrenProjects();
		QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
		return true;
	}
	return false;
}

bool QMakeXUPItem::saveProject( const QString& s, const QString& v )
{
	QByteArray a = QMake2XUP::convertToPro( domDocument(), v );
	if ( !a.isNull() )
	{
		QFile f( s.isEmpty() ? projectFilePath() : s );
		if ( !f.open( QIODevice::WriteOnly ) )
			return false;
		f.resize( 0 );
		if ( !( f.write( a ) != -1 ) )
			return false;
		mProjectFilePath = f.fileName();
		setModified( false );
		return true;
	}
	return false;
}

void QMakeXUPItem::addFiles( const QStringList& files, XUPItem* scope, const QString& op )
{
	// abort if no files
	if ( files.isEmpty() )
		return;
	
	// set scope
	if ( !scope )
		scope = this;
	
	// remember if it's a project container or not
	bool container = isProjectContainer();
	
	foreach ( QString file, files )
	{
		QStringList vnl;
		// get variables that can handle this file
		if ( container )
		{
			// check project extension
			foreach ( QString vn, mQMakeXUPItemInfos.Suffixes.keys() )
				if ( QDir::match( mQMakeXUPItemInfos.Suffixes[vn], file ) )
					vnl << "SUBDIRS";
			
			// if no variable it mean not proejct file, process next file
			if ( vnl.isEmpty() )
				continue;
		}
		else
		{
			// variables names that can handle this suffix
			foreach ( QString vn, mQMakeXUPItemInfos.VariableSuffixes.keys() )
			if ( QDir::match( mQMakeXUPItemInfos.VariableSuffixes[vn], file ) )
				vnl << vn;
		}
		
		// request variable name to user if more than 1
		QString vn;
		if ( vnl.count() > 1 )
		{
			bool ok;
			vn = QInputDialog::getItem( QApplication::activeWindow(), tr( "Choose variable..." ), tr( "More than one variable can handle this file,\nplease select the variable you want to use for this file :\n%1" ).arg( QFileInfo( file ).fileName() ), vnl, 0, false, &ok );
			if ( !ok )
				vn.clear();
		}
		else
			vn = vnl.value( 0 );
		if ( vn.isEmpty() )
			vn = "OTHER_FILES";
		
		// get variable item
		XUPItem* vit = 0;
		foreach ( XUPItem* cit, scope->children( false, true ) )
		{
			if ( cit->isType( "variable" ) && cit->defaultValue() == vn && cit->value( "operator", "=" ) == op )
			{
				vit = cit;
				break;
			}
		}
		
		// check variable exists or not
		bool exists = vit;
	
		// create variable if needed
		if ( !exists )
		{
			vit = clone( false );
			vit->setDomElement( mDomElement.ownerDocument().createElement( "variable" ) );
			scope->domElement().appendChild( vit->domElement() );
			vit->setValue( vit->valueName(), vn );
			vit->setValue( "operator", op );
		}
	
		// set variable multiline
		vit->setValue( "multiline", "true" );
	
		// get all files
		QStringList existingFiles = vit->files();
	
		// add file
		QString fp = filePath( file );
		if ( !existingFiles.contains( fp ) )
		{
			XUPItem* it = clone( false );
			it->setDomElement( mDomElement.ownerDocument().createElement( "value" ) );
			vit->domElement().appendChild( it->domElement() );
			it->setValue( it->valueName(), relativeFilePath( fp ) );
			vit->appendRow( it );
		}
	
		// append item to tree if needed
		if ( !exists )
			scope->appendRow( vit );
		
		// open sub project if needed
		if ( vn == "SUBDIRS" && container )
		{
			XUPItem* pi = clone( false );
			if ( pi->loadProject( file ) )
				project()->appendRow( pi );
			else
				delete pi;
		}
	}
}

void QMakeXUPItem::removeFiles( const QStringList& files, XUPItem* scope, const QString& op )
{
	// abort if no files
	if ( files.isEmpty() )
		return;
	
	// set scope
	if ( !scope )
		scope = this;
	
	// check variables
	foreach ( XUPItem* vit, scope->children( false, true ) )
	{
		if ( vit->isType( "variable" ) && vit->value( "operator" ) == op )
		{
			// check each child
			foreach ( XUPItem* cit, vit->children( false, false ) )
				if ( cit->isType( "value" ) && files.contains( cit->filePath() ) )
					cit->remove();
			
			// remove variable item if emptyline
			if ( !vit->hasChildren() )
				vit->remove();
		}
	}
}

QString QMakeXUPItem::filePath( const QString& s ) const
{
	if ( s.isEmpty() && isType( "value" ) )
	{
		const QString v = parent()->defaultValue();
		if ( ( fileVariables().contains( v ) || pathVariables().contains( v ) ) && !defaultValue().isEmpty() )
		{
			const QString pp = projectPath().append( "/%1" );
			const QString iv = defaultInterpretedValue();
			if ( QFile::exists( pp.arg( iv ) ) )
				return QFileInfo( pp.arg( iv ) ).absoluteFilePath();
			
			// looking in paths of project
			QString ps = interpretedVariable( "INCLUDEPATH", this );
			ps += " " +interpretedVariable( "DEPENDPATH", this );
			ps += " " +interpretedVariable( "VPATH", this );
			foreach ( QString p, splitFiles( ps.trimmed() ) )
			{
				p.replace( '\\', '/' );
				if ( ( p.startsWith( '/' ) || p.mid( 1, 2 ) == ":/" ) && QFile::exists( QString( p ).append( "/%1" ).arg( iv ) ) )
					return QFileInfo( QString( p ).append( "/%1" ).arg( iv ) ).absoluteFilePath();
				else if ( QFile::exists( pp.arg( p ).append( "/%1" ).arg( iv ) ) )
					return QFileInfo( pp.arg( p ).append( "/%1" ).arg( iv ) ).absoluteFilePath();
			}
		}
	}
	return XUPItem::filePath( s );
}

QStringList QMakeXUPItem::splitFiles( const QString& s ) const
{
	QString fs = QString( s ).replace( projectPath(), "$pp$" );
	QStringList l;
	QRegExp rx = QRegExp( "(?:\"[^\"]+\")|([^ \"]+)" );
	int p = 0;
	while ( ( p = rx.indexIn( fs, p ) ) != -1 )
	{
		l << rx.capturedTexts().value( 0 ).replace( '"', "" ).trimmed().replace( "$pp$", projectPath() );
		p += rx.matchedLength();
	}
	return l;
}

BuilderPlugin* QMakeXUPItem::builder( const QString& plugin ) const
{ return XUPItem::builder( plugin ); }

CompilerPlugin* QMakeXUPItem::compiler( const QString& plugin ) const
{ return XUPItem::compiler( plugin ); }

DebuggerPlugin* QMakeXUPItem::debugger( const QString& plugin ) const
{ return XUPItem::debugger( plugin ); }

InterpreterPlugin* QMakeXUPItem::interpreter( const QString& plugin ) const
{ return XUPItem::interpreter( plugin ); }

void QMakeXUPItem::installCommands()
{
	// get plugins
	BuilderPlugin* bp = builder();
	CompilerPlugin* cp = compiler();
	
	// temp command
	pCommand cmd;
	
	// build command
	if ( bp )
	{
		cmd = bp->buildCommand();
		if ( cp )
			cmd.addParsers( cp->compileCommand().parsers() );
	}
	cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
	cmd.setProject( this );
	const pCommand cmdBuild = cmd;
	
	// get qt version
	QtVersionManager mQtManager;
	QtVersion mQtVersion = mQtManager.version( projectSettingsValue( "QT_VERSION" ) );
	
	// evaluate some variables
	QString s;
	s = interpretedVariable( "TARGET" );
	if ( s.isEmpty() )
		s = QFileInfo( projectFilePath() ).baseName();
	const QString target = s;
	s = interpretedVariable( "DESTDIR" );
	if ( s.isEmpty() )
		s = interpretedVariable( "DLLDESTDIR" );
#ifdef Q_OS_WIN
	if ( !s.left( 2 ).endsWith( ':' ) )
		s.prepend( "$cpp$/" );
#else
	if ( !s.startsWith( '/' ) )
		s.prepend( "$cpp$/" );
#endif
	if ( s.endsWith( '/' ) )
		s.chop( 1 );
	const QString destdir = s;
	
	// compiler
	if ( bp && cmdBuild.isValid() )
	{
		// build debug
		cmd = cmdBuild;
		cmd.setText( tr( "Build Debug" ) );
		cmd.setArguments( "debug" );
		addCommand( cmd, "mBuilder/mBuild" );
		
		// build release
		cmd = cmdBuild;
		cmd.setText( tr( "Build Release" ) );
		cmd.setArguments( "release" );
		addCommand( cmd, "mBuilder/mBuild" );
		
		// build all
		cmd = cmdBuild;
		cmd.setText( tr( "Build All" ) );
		cmd.setArguments( "all" );
		addCommand( cmd, "mBuilder/mBuild" );
		
		// clean
		cmd = cmdBuild;
		cmd.setText( tr( "Clean" ) );
		cmd.setArguments( "clean" );
		addCommand( cmd, "mBuilder/mClean" );
		
		// distclean
		cmd = cmdBuild;
		cmd.setText( tr( "Distclean" ) );
		cmd.setArguments( "distclean" );
		addCommand( cmd, "mBuilder/mClean" );
		
		// add qt commands only if possible
		if ( mQtVersion.isValid() )
		{
			// qmake command
			cmd = pCommand();
			cmd.setText( tr( "QMake" ) );
			cmd.setCommand( mQtVersion.qmake() );
			cmd.setArguments( mQtVersion.qmakeParameters()/*.append( " $cp$" )*/ );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
			cmd.setProject( this );
			addCommand( cmd, "mBuilder" );
			
			// lupdate command
			cmd = pCommand();
			cmd.setText( tr( "lupdate" ) );
			cmd.setCommand( mQtVersion.lupdate() );
			cmd.setArguments( "$cp$" );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
			cmd.setProject( this );
			addCommand( cmd, "mBuilder" );
			
			// lrelease command
			cmd = pCommand();
			cmd.setText( tr( "lrelease" ) );
			cmd.setCommand( mQtVersion.lrelease() );
			cmd.setArguments( "$cp$" );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setUserData( reinterpret_cast<quintptr>( &mCommands ) );
			cmd.setProject( this );
			addCommand( cmd, "mBuilder" );
		
			// rebuild debug
			cmd = cmdBuild;
			cmd.setText( tr( "Rebuild Debug" ) );
			cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build Debug" ) ).join( ";" ) );
			cmd.setArguments( QString() );
			addCommand( cmd, "mBuilder/mRebuild" );
			
			// rebuild release
			cmd = cmdBuild;
			cmd.setText( tr( "Rebuild Release" ) );
			cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build Release" ) ).join( ";" ) );
			cmd.setArguments( QString() );
			addCommand( cmd, "mBuilder/mRebuild" );
			
			// rebuild all
			cmd = cmdBuild;
			cmd.setText( tr( "Rebuild All" ) );
			cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build All" ) ).join( ";" ) );
			cmd.setArguments( QString() );
			addCommand( cmd, "mBuilder/mRebuild" );
			
			// simple rebuild call
			cmd = cmdBuild;
			cmd.setText( tr( "Rebuild" ) );
			cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build" ) ).join( ";" ) );
			cmd.setArguments( QString() );
			addCommand( cmd, "mBuilder/mRebuild" );
		}
		else
			pMonkeyStudio::warning( tr( "Warning..." ), tr( "Some actions can't be created, because there is no default Qt version setted, please go in your project settings to fix this." ) );
		
		// execute debug
		cmd = cmdBuild;
		cmd.setText( tr( "Execute Debug" ) );
		cmd.setCommand( target );
		cmd.setArguments( QString() );
		cmd.setWorkingDirectory( destdir != "$cpp$" ? destdir : destdir +"/debug" );
		addCommand( cmd, "mBuilder/mExecute" );
		
		// execute release
		cmd = cmdBuild;
		cmd.setText( tr( "Execute Release" ) );
		cmd.setCommand( target );
		cmd.setArguments( QString() );
		cmd.setWorkingDirectory( destdir != "$cpp$" ? destdir : destdir +"/release" );
		addCommand( cmd, "mBuilder/mExecute" );
	}
	
	// install defaults commands
	XUPItem::installCommands();
}
