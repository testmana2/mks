#include "QMakeProjectItem.h"
#include "QMake2XUP.h"
#include "../../core/XUPProjectItemInfos.h"

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

QMakeProjectItem::QMakeProjectItem()
	: XUPProjectItem()
{
}

QMakeProjectItem::~QMakeProjectItem()
{
}

void QMakeProjectItem::registerProjectType() const
{
	// get proejct type
	int pType = projectType();
	
	// register it
	mXUPProjectInfos->unRegisterType( pType );
	mXUPProjectInfos->registerType( pType );
	
	// values
	const QString mPixmapsPath = ":/qmakeitems";
	const QStringList mOperators = QStringList( "=" ) << "+=" << "-=" << "*=" << "~=";
	const QStringList mFilteredVariables = QStringList( "SUBDIRS" ) << "FORMS" << "FORMS3"
		<< "HEADERS" << "SOURCES" << "OBJECTIVE_SOURCES" << "TRANSLATIONS" << "RESOURCES"
		<< "RC_FILE" << "RES_FILE" << "DEF_FILE" << "INCLUDEPATH" << "DEPENDPATH" << "VPATH"
		<< "LIBS" << "DEFINES" << "OTHER_FILES";
	const QStringList mFileVariables = QStringList( "FORMS" ) << "FORMS3" << "HEADERS"
		<< "SOURCES" << "OBJECTIVE_SOURCES" << "TRANSLATIONS" << "RESOURCES"
		<< "RC_FILE" << "RES_FILE" << "DEF_FILE" << "SUBDIRS" << "OTHER_FILES";
	const QStringList mPathVariables = QStringList( "INCLUDEPATH" ) << "DEPENDPATH"
		<< "VPATH";
	const StringStringListList mSuffixes = StringStringListList()
		<< qMakePair( tr( QT_TR_NOOP( "Qt Project" ) ), QStringList( "*.pro" ) )
		<< qMakePair( tr( QT_TR_NOOP( "Qt Include Project" ) ), QStringList( "*.pri" ) );
	const StringStringList mVariableLabels = StringStringList()
		<< qMakePair( QString( "FORMS" ), tr( QT_TR_NOOP( "Forms Files" ) ) )
		<< qMakePair( QString( "FORMS3" ), tr( QT_TR_NOOP( "Forms 3 Files" ) ) )
		<< qMakePair( QString( "HEADERS" ), tr( QT_TR_NOOP( "Headers Files" ) ) )
		<< qMakePair( QString( "SOURCES" ), tr( QT_TR_NOOP( "Sources Files" ) ) )
		<< qMakePair( QString( "OBJECTIVE_SOURCES" ), tr( QT_TR_NOOP( "Objective Sources Files" ) ) )
		<< qMakePair( QString( "TRANSLATIONS" ), tr( QT_TR_NOOP( "Qt Translations Files" ) ) )
		<< qMakePair( QString( "RESOURCES" ), tr( QT_TR_NOOP( "Qt Resources Files" ) ) )
		<< qMakePair( QString( "RC_FILE" ), tr( QT_TR_NOOP( "Resources Files" ) ) )
		<< qMakePair( QString( "RES_FILE" ), tr( QT_TR_NOOP( "Compiled Resources Files" ) ) )
		<< qMakePair( QString( "DEF_FILE" ), tr( QT_TR_NOOP( "Definitions Files" ) ) )
		<< qMakePair( QString( "SUBDIRS" ), tr( QT_TR_NOOP( "Sub Projects" ) ) )
		<< qMakePair( QString( "INCLUDEPATH" ), tr( QT_TR_NOOP( "Includes Paths" ) ) )
		<< qMakePair( QString( "DEPENDPATH" ), tr( QT_TR_NOOP( "Depends Paths" ) ) )
		<< qMakePair( QString( "VPATH" ), tr( QT_TR_NOOP( "Virtuals Paths" ) ) )
		<< qMakePair( QString( "LIBS" ), tr( QT_TR_NOOP( "Libraries Files" ) ) )
		<< qMakePair( QString( "DEFINES" ), tr( QT_TR_NOOP( "Defines" ) ) )
		<< qMakePair( QString( "OTHER_FILES" ), tr( QT_TR_NOOP( "Other Files" ) ) );
	const StringStringList mVariableIcons = StringStringList()
		<< qMakePair( QString( "FORMS" ), QString( "forms" ) )
		<< qMakePair( QString( "FORMS3" ), QString( "forms3" ) )
		<< qMakePair( QString( "HEADERS" ), QString( "headers" ) )
		<< qMakePair( QString( "SOURCES" ), QString( "sources" ) )
		<< qMakePair( QString( "OBJECTIVE_SOURCES" ), QString( "objective_sources" ) )
		<< qMakePair( QString( "TRANSLATIONS" ), QString( "translations" ) )
		<< qMakePair( QString( "RESOURCES" ), QString( "resources" ) )
		<< qMakePair( QString( "RC_FILE" ), QString( "rc_file" ) )
		<< qMakePair( QString( "RES_FILE" ), QString( "res_file" ) )
		<< qMakePair( QString( "DEF_FILE" ), QString( "def_file" ) )
		<< qMakePair( QString( "SUBDIRS" ), QString( "project" ) )
		<< qMakePair( QString( "INCLUDEPATH" ), QString( "includepath" ) )
		<< qMakePair( QString( "DEPENDPATH" ), QString( "dependpath" ) )
		<< qMakePair( QString( "VPATH" ), QString( "vpath" ) )
		<< qMakePair( QString( "LIBS" ), QString( "libs" ) )
		<< qMakePair( QString( "DEFINES" ), QString( "defines" ) )
		<< qMakePair( QString( "OTHER_FILES" ), QString( "other_files" ) );
#warning need use pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
	const QStringList cf = QStringList()
		<< "*.h" << "*.c"
		<< "*.hh" << "*.cc"
		<< "*.hpp" << "*.cpp"
		<< "*.h++" << "*.c++"
		<< "*.hxx" << "*.cxx";
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
#warning need use pMonkeyStudio::availableImageFormats();
	const QStringList pf = QStringList( "*.png" ) << "*.jpg" << "*.bmp" << "*.gif";
	// PROJECT filters
	QStringList pjf;
	foreach ( const PairStringStringList& p, mSuffixes )
		pjf << p.second;
	// Variable suffixes
	const StringStringListList mVariableSuffixes = StringStringListList()
		<< qMakePair( QString( "HEADERS" ), hf )
		<< qMakePair( QString( "SOURCES" ), sf )
		<< qMakePair( QString( "YACCSOURCES" ), yf )
		<< qMakePair( QString( "LEXSOURCES" ), lf )
		<< qMakePair( QString( "IMAGES" ), pf )
		<< qMakePair( QString( "OBJECTIVE_SOURCES" ), QStringList( "*.m" ) << "*.mm" )
		<< qMakePair( QString( "FORMS" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "FORMS3" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "TRANSLATIONS" ), QStringList( "*.ts" ) )
		<< qMakePair( QString( "RESOURCES" ), QStringList( "*.qrc" ) )
		<< qMakePair( QString( "DEF_FILE" ), QStringList( "*.def" ) )
		<< qMakePair( QString( "RC_FILE" ), QStringList( "*.rc" ) )
		<< qMakePair( QString( "RES_FILE" ), QStringList( "*.res" ) );
	
	// register values
	mXUPProjectInfos->registerPixmapsPath( pType, mPixmapsPath );
	mXUPProjectInfos->registerOperators( pType, mOperators );
	mXUPProjectInfos->registerFilteredVariables( pType, mFilteredVariables );
	mXUPProjectInfos->registerFileVariables( pType, mFileVariables );
	mXUPProjectInfos->registerPathVariables( pType, mFileVariables );
	mXUPProjectInfos->registerSuffixes( pType, mSuffixes );
	mXUPProjectInfos->registerVariableLabels( pType, mVariableLabels );
	mXUPProjectInfos->registerVariableIcons( pType, mVariableIcons );
	mXUPProjectInfos->registerVariableSuffixes( pType, mVariableSuffixes );
}

QStringList splitSubdirs( const QString& value )
{
	QStringList tmpValues = value.split(" ");
	bool inStr = false;
	QStringList multivalues;
	QString ajout;

	for(int ku = 0;ku < tmpValues.size();ku++)
	{
		if(tmpValues.value(ku).startsWith('"') )
				inStr = true;
		if(inStr)
		{
			if(ajout != "")
					ajout += " ";
			ajout += tmpValues.value(ku);
			if(tmpValues.value(ku).endsWith('"') )
			{
					multivalues += ajout;
					ajout = "";
					inStr = false;
			}
		}
		else
		{
			multivalues += tmpValues.value(ku);
		}
	}

	return multivalues;
}

void QMakeProjectItem::customRowCount( XUPItem* item ) const
{
	if ( item->type() == XUPItem::Variable && item->attribute( "name" ) == "SUBDIRS" )
	{
		if ( !item->temporaryValue( "subdirsHandled", false ).toBool() )
		{
			QStringList subdirs;
			
			for ( int i = 0; i < item->count(); i++ )
			{
				XUPItem* cit = item->child( i );
				if ( cit->type() == XUPItem::Value )
				{
					subdirs << splitSubdirs( cit->attribute( "content" ) );
				}
			}
			
			if ( !subdirs.isEmpty() )
			{
				foreach ( const QString& subdir, subdirs )
				{
					// generate project filepath
					QString fn = subdir;
					if ( QDir::isRelativePath( subdir ) )
						fn = filePath( subdir );
					QFileInfo fi( fn );
					if ( fi.isDir() )
						fi.setFile( fn, QString( "%1.pro" ).arg( subdir ) );
					// open project
					XUPProjectItem* project = newItem();
					if ( project->open( fi.absoluteFilePath(), attribute( "encoding" ) ) )
					{
						int count = item->count();
						project->setParent( item );
						project->setRow( count );
						item->setChild( count, project );
					}
					else
					{
						delete project;
					}
				}
			}
			
			item->setTemporaryValue( "subdirsHandled", true );
		}
	}
}

bool QMakeProjectItem::open( const QString& fileName, const QString& encoding )
{
	QString buffer = QMake2XUP::convertFromPro( fileName, encoding );
	
	// parse content
	QString errorMsg;
	int errorLine;
	int errorColumn;
	if ( !mDocument.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) )
	{
		setLastError( QString( "%1 on line: %2, column: %3" ).arg( errorMsg ).arg( errorLine ).arg( errorColumn ) );
		return false;
	}
	
	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
	if ( mDomElement.isNull() )
	{
		setLastError("no project node" );
		return false;
	}
	
	// all is ok
	setAttribute( "encoding", encoding );
	setTemporaryValue( "filename", fileName );
	setLastError( QString::null );
	mRowNumber = 0;
	
	return true;
}

void QMakeProjectItem::close()
{
}

QString QMakeProjectItem::interpretVariable( const QString& variableName, const XUPItem* callerItem, const QString& defaultValue ) const
{
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read from environment when qmake run
		$(QTDIR) : read from generated makefile
	*/
	
	QString dv = QString( variableName ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	// environment var
	if ( variableName.startsWith( "$$(" ) || dv == "PWD" )
		return dv != "PWD" ? qgetenv( dv.toLocal8Bit().constData() ) : ( callerItem ? callerItem->project()->path() : path() );
	else if ( variableName.startsWith( "$(" ) )
		qWarning() << "Don't know how to proceed: " << variableName;
	else if ( variableName.startsWith( "$$[" ) )
		qWarning() << "Don't know how to proceed: " << variableName;
	else
	{
		QString v;
		QList<XUPItem*> variables = getVariables( this, dv, callerItem );
		foreach ( XUPItem* cit, variables )
		{
			if ( callerItem && cit == callerItem )
				return v;
			
			const QString o = cit->attribute( "operator", "=" );
			QString cv;
			for ( int i = 0; i < cit->count(); i++ )
			{
				XUPItem* ccit = cit->child( i );
				if ( ccit->type() == XUPItem::Value )
				{
					cv += interpretValue( ccit, "content" ) +" ";
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
		return v;
	}
	
	return defaultValue;
}

QString QMakeProjectItem::interpretValue( XUPItem* callerItem, const QString& attribute ) const
{
	QRegExp rx( "\\$\\$?[\\{\\(\\[]?(\\w+(?!\\w*\\s*[()]))[\\}\\)\\]]?" );
	const QString content = callerItem->attribute( attribute );
	QString value = content;
	int pos = 0;
	while ( ( pos = rx.indexIn( content, pos ) ) != -1 )
	{
		value.replace( rx.cap( 0 ), interpretVariable( rx.cap( 0 ), callerItem ) );
		pos += rx.matchedLength();
	}
	return value;
}
