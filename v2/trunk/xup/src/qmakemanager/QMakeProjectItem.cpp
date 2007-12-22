#include "QMakeProjectItem.h"
#include "XUPManager.h"
#include "QMake2XUP.h"

#include <QIcon>

using namespace XUPManager;

QIcon getIcon( const QString& s )
{ return QIcon( QString( ":/icons/icons/%1.png" ).arg( s ) ); }

QMakeProjectItem::QMakeProjectItem( const QDomElement& e, const QString& s, bool b )
	: ProjectItem( e, s, b )
{}

void QMakeProjectItem::registerItem()
{
	// registering...
	// variables based on files
	registerFileVariables( "FORMS" );
	registerFileVariables( "FORMS3" );
	registerFileVariables( "HEADERS" );
	registerFileVariables( "SOURCES" );
	registerFileVariables( "TRANSLATIONS" );
	registerFileVariables( "RESOURCES" );
	registerFileVariables( "SUBDIRS" );
	// variables based on paths
	registerPathVariables( "INCLUDEPATH" );
	registerPathVariables( "DEPENDPATH" );
	registerPathVariables( "VPATH" );
	// variables labels
	registerVariableLabels( "FORMS", tr( "Forms Files" ) );
	registerVariableLabels( "FORMS3", tr( "Forms 3 Files" ) );
	registerVariableLabels( "HEADERS", tr( "Headers Files" ) );
	registerVariableLabels( "SOURCES", tr( "Sources Files" ) );
	registerVariableLabels( "TRANSLATIONS", tr( "Translations Files" ) );
	registerVariableLabels( "RESOURCES", tr( "Resources Files" ) );
	registerVariableLabels( "SUBDIRS", tr( "Sub Projects" ) );
	// variables labels
	registerVariableLabels( "INCLUDEPATH", tr( "Includes Paths" ) );
	registerVariableLabels( "DEPENDPATH", tr( "Depends Paths" ) );
	registerVariableLabels( "VPATH", tr( "Virtuals Paths" ) );
	registerVariableLabels( "LIBS", tr( "Libraries Files" ) );
	registerVariableLabels( "DEFINES", tr( "Defines" ) );
	registerVariableLabels( "RC_FILE", tr( "Resources Files" ) );
	registerVariableLabels( "RES_FILE", tr( "Compiled Resources Files" ) );
	registerVariableLabels( "DEF_FILE", tr( "Definitions Files" ) );
	// variables icons
	registerVariableIcons( "FORMS", getIcon( "forms" ) );
	registerVariableIcons( "FORMS3", getIcon( "forms3" ) );
	registerVariableIcons( "HEADERS", getIcon( "headers" ) );
	registerVariableIcons( "SOURCES", getIcon( "sources" ) );
	registerVariableIcons( "TRANSLATIONS", getIcon( "translations" ) );
	registerVariableIcons( "RESOURCES", getIcon( "resources" ) );
	registerVariableIcons( "SUBDIRS", getIcon( "project" ) );
	// variables icons
	registerVariableIcons( "INCLUDEPATH", getIcon( "includepath" ) );
	registerVariableIcons( "DEPENDPATH", getIcon( "dependpath" ) );
	registerVariableIcons( "VPATH", getIcon( "vpath" ) );
	registerVariableIcons( "LIBS", getIcon( "libs" ) );
	registerVariableIcons( "DEFINES", getIcon( "defines" ) );
	registerVariableIcons( "RC_FILE", getIcon( "rc_file" ) );
	registerVariableIcons( "RES_FILE", getIcon( "res_file" ) );
	registerVariableIcons( "DEF_FILE", getIcon( "def_file" ) );
	//
	qWarning( qPrintable( tr( "QMakeProjectItem Registered" ) ) ); 
}

QMakeProjectItem* QMakeProjectItem::clone() const
{ return new QMakeProjectItem( domElement(), projectFilePath(), modified() ); }

void QMakeProjectItem::appendRow( QMakeProjectItem* it )
{ ProjectItem::appendRow( it ); }

void QMakeProjectItem::checkChildrenProjects()
{
	foreach ( ProjectItem* it, children( true, true ) )
	{
		QString s;
		if ( it->value( "type" ) == "variable" && it->value( "name" ) == "SUBDIRS" )
		{
			foreach ( ProjectItem* cit, it->children( false, true ) )
			{
				s = cit->filePath();
				QFileInfo fi( s );
				if ( fi.isDir() )
					s.append( QString( "/%1.pro" ).arg( fi.fileName() ) );
				
				if ( QFile::exists( s ) )
				{
					QMakeProjectItem* pi = new QMakeProjectItem;
					if ( pi->loadProject( s ) )
						it->parent()->appendRow( pi );
					else
						delete pi;
				}
			}
		}
		else if ( it->value( "type" ) == "function" && it->defaultValue().startsWith( "include" ) )
		{
			s = it->defaultValue();
			s.remove( 0, s.indexOf( '(' ) +1 );
			s.chop( s.length() -s.lastIndexOf( ')' ) );
			s = it->filePath( s.trimmed() );
			
			if ( QFile::exists( s ) )
			{
				QMakeProjectItem* pi = new QMakeProjectItem;
				if ( pi->loadProject( s ) )
					it->appendRow( pi );
				else
					delete pi;
			}
		}
	}
}

bool QMakeProjectItem::loadProject( const QString& s, const QString& v )
{
	if ( loadXUP( this, QMake2XUP::convertFromPro( s, v ), v ) )
	{
		mProjectFilePath = s;
		setModified( false );
		checkChildrenProjects();
		return true;
	}
	return false;
}

bool QMakeProjectItem::saveProject( const QString& s, const QString& v )
{
	if ( QMake2XUP::convertToPro( toDomDocument(), s.isEmpty() ? projectFilePath() : s, v ) )
	{
		mProjectFilePath = s.isEmpty() ? projectFilePath() : s;
		setModified( false );
		return true;
	}
	return false;
}
