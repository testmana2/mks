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

QString QMakeProjectItem::interpretVariable( const QString& variableName, const XUPItem* callerItem, const QString& defaultValue ) const
{
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read from environment when qmake run
		$(QTDIR) : read from generated makefile
	*/
	
	QString name = QString( variableName ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	QList<QStringList> value;
	
	// environment var
	if ( variableName.startsWith( "$$(" ) || name == "PWD" )
	{
		return name != "PWD" ? qgetenv( name.toLocal8Bit().constData() ) : ( callerItem ? callerItem->project()->path() : path() );
	}
	else if ( variableName.startsWith( "$(" ) )
	{
		const_cast<QMakeProjectItem*>( this )->setLastError( tr( "Don't know how to interpret: %1" ).arg( variableName ) );
	}
	else if ( variableName.startsWith( "$$[" ) )
	{
		const_cast<QMakeProjectItem*>( this )->setLastError( tr( "Don't know how to interpret: %1" ).arg( variableName ) );
	}
	else
	{
		QList<XUPItem*> variableItems = getVariables( this, name, callerItem );
		foreach ( XUPItem* variableItem, variableItems )
		{
			const QString op = variableItem->attribute( "operator", "=" );
			QStringList tmp;
			for ( int i = 0; i < variableItem->childCount(); i++ )
			{
				XUPItem* valueItem = variableItem->child( i );
				if ( valueItem->type() == XUPItem::Value ||
					valueItem->type() == XUPItem::File ||
					valueItem->type() == XUPItem::Path
				)
				{
					tmp << interpretValue( valueItem, "content" );
				}
			}
			
			if ( op == "=" )
			{
				value = QList<QStringList>() << QStringList( tmp );
			}
			else if ( op == "-=" )
			{
				value.removeAll( tmp );
			}
			else if ( op == "+=" )
			{
				value << tmp;
			}
			else if ( op == "*=" )
			{
				if ( !value.contains( tmp ) )
					value << tmp;
			}
			else if ( op == "~=" )
			{
				const_cast<QMakeProjectItem*>( this )->setLastError( tr( "Don't know how to interpret ~= operator" ) );
			}
		}
	}
	
	QStringList result;
	foreach ( const QStringList& values, value )
		result << values;
	
	return result.isEmpty() ? defaultValue : result.join( " " );
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

QFileInfoList QMakeProjectItem::findFile( const QString& partialFilePath ) const
{	
	return XUPProjectItem::findFile( partialFilePath );
}

void QMakeProjectItem::registerProjectType() const
{
	// get proejct type
	int pType = projectType();
	
	// register it
	mXUPProjectInfos->unRegisterType( pType );
	mXUPProjectInfos->registerType( pType, const_cast<QMakeProjectItem*>( this ) );
	
	// values
	const QString mPixmapsPath = ":/qmakeitems";
	const QStringList mOperators = QStringList( "=" ) << "+=" << "-=" << "*=" << "~=";
	const QStringList mFilteredVariables = QStringList() << "FORMS" << "FORMS3"
		<< "HEADERS" << "SOURCES" << "OBJECTIVE_SOURCES" << "TRANSLATIONS" << "RESOURCES"
		<< "RC_FILE" << "RES_FILE" << "DEF_FILE" << "INCLUDEPATH" << "DEPENDPATH" << "VPATH"
		<< "LIBS" << "DEFINES" << "OTHER_FILES";
	const QStringList mFileVariables = QStringList( "FORMS" ) << "FORMS3" << "HEADERS"
		<< "SOURCES" << "OBJECTIVE_SOURCES" << "TRANSLATIONS" << "RESOURCES"
		<< "RC_FILE" << "RES_FILE" << "DEF_FILE" << "SUBDIRS" << "OTHER_FILES";
	const QStringList mPathVariables = QStringList( "INCLUDEPATH" ) << "DEPENDPATH"
		<< "VPATH";
	const StringStringListList mSuffixes = StringStringListList()
		<< qMakePair( tr( "Qt Project" ), QStringList( "*.pro" ) )
		<< qMakePair( tr( "Qt Include Project" ), QStringList( "*.pri" ) );
	const StringStringList mVariableLabels = StringStringList()
		<< qMakePair( QString( "FORMS" ), tr( "Forms Files" ) )
		<< qMakePair( QString( "FORMS3" ), tr( "Forms 3 Files" ) )
		<< qMakePair( QString( "HEADERS" ), tr( "Headers Files" ) )
		<< qMakePair( QString( "SOURCES" ), tr( "Sources Files" ) )
		<< qMakePair( QString( "OBJECTIVE_SOURCES" ), tr( "Objective Sources Files" ) )
		<< qMakePair( QString( "TRANSLATIONS" ), tr( "Qt Translations Files" ) )
		<< qMakePair( QString( "RESOURCES" ), tr( "Qt Resources Files" ) )
		<< qMakePair( QString( "RC_FILE" ), tr( "Resources Files" ) )
		<< qMakePair( QString( "RES_FILE" ), tr( "Compiled Resources Files" ) )
		<< qMakePair( QString( "DEF_FILE" ), tr( "Definitions Files" ) )
		<< qMakePair( QString( "SUBDIRS" ), tr( "Sub Projects" ) )
		<< qMakePair( QString( "INCLUDEPATH" ), tr( "Includes Paths" ) )
		<< qMakePair( QString( "DEPENDPATH" ), tr( "Depends Paths" ) )
		<< qMakePair( QString( "VPATH" ), tr( "Virtuals Paths" ) )
		<< qMakePair( QString( "LIBS" ), tr( "Libraries Files" ) )
		<< qMakePair( QString( "DEFINES" ), tr( "Defines" ) )
		<< qMakePair( QString( "OTHER_FILES" ), tr( "Other Files" ) );
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
	mXUPProjectInfos->registerPathVariables( pType, mPathVariables );
	mXUPProjectInfos->registerSuffixes( pType, mSuffixes );
	mXUPProjectInfos->registerVariableLabels( pType, mVariableLabels );
	mXUPProjectInfos->registerVariableIcons( pType, mVariableIcons );
	mXUPProjectInfos->registerVariableSuffixes( pType, mVariableSuffixes );
}

void QMakeProjectItem::customRowCount( XUPItem* item ) const
{
	if ( item->type() == XUPItem::Variable && item->attribute( "name" ) == "SUBDIRS" )
	{
		if ( !item->temporaryValue( "subdirsHandled", false ).toBool() )
		{
			item->setTemporaryValue( "subdirsHandled", true );
			QStringList subdirs;
			
			for ( int i = 0; i < item->childCount(); i++ )
			{
				XUPItem* cit = item->child( i );
				if ( cit->type() == XUPItem::File )
				{
					subdirs << splitMultiLineValue( cit->attribute( "content" ) );
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
					XUPProjectItem* project = newProject();
					if ( project->open( fi.absoluteFilePath(), attribute( "encoding" ) ) )
					{
						item->addChild( project );
					}
					else
					{
						delete project;
					}
				}
			}
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
	setTemporaryValue( "fileName", fileName );
	setLastError( QString::null );
	
	return true;
}

void QMakeProjectItem::close()
{
}
