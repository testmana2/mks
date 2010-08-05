#include "XUPProjectItem.h"
#include "XUPProjectModel.h"
#include "MonkeyCore.h"
#include "pMenuBar.h"
#include "PluginsManager.h"
#include "pMonkeyStudio.h"
#include "UIMain.h"

#include <QTextCodec>
#include <QDir>
#include <QRegExp>
#include <QProcess>
#include <QFileDialog>
#include <QLibrary>

#include <QDebug>

const QString XUP_VERSION = "1.1.0";

bool XUPProjectItem::mFoundCallerItem = false;

XUPProjectItem::XUPProjectItem()
	: XUPItem( QDomElement(), 0 )
{
}

XUPProjectItem::~XUPProjectItem()
{
}

void XUPProjectItem::setLastError( const QString& error )
{
	mLastError = error;
}

QString XUPProjectItem::lastError() const
{
	return mLastError;
}

QString XUPProjectItem::fileName() const
{
	return mFileName;
}

QString XUPProjectItem::path() const
{
	return QFileInfo( fileName() ).path();
}

QString XUPProjectItem::filePath( const QString& fn ) const
{
	if ( fn.isEmpty() )
		return QString::null;
	QString fname = QFileInfo( fn ).isRelative() ? path().append( "/" ).append( fn ) : fn;
	return QDir::cleanPath( fname );
}

QString XUPProjectItem::relativeFilePath( const QString& fileName ) const
{
	QDir dir( path() );
	return dir.relativeFilePath( fileName );
}

QStringList XUPProjectItem::sourceFiles() const
{
	return QStringList();
}

QStringList XUPProjectItem::topLevelProjectSourceFiles() const
{
	QSet<QString> files;

	XUPProjectItemList projects = childrenProjects( true );

	foreach ( XUPProjectItem* project, projects )
	{
		const QStringList sources = project->sourceFiles();

		foreach ( const QString& source, sources )
		{
			files << source;
		}
	}

	return files.toList();
}

QString XUPProjectItem::matchingPath( const QString& left, const QString& right ) const
{
	QString result;
	for ( int i = 1; i < left.count() +1; i++ )
	{
		result = left.left( i );
		if ( !right.startsWith( result ) )
		{
			result.chop( 1 );
			break;
		}
	}

	if ( QDir::drives().contains( result ) || result.isEmpty() )
	{
		return QString::null;
	}

	return result;
}

QFileInfoList XUPProjectItem::findFile( const QString& partialFilePath ) const
{
	QFileInfoList files;
	QString searchFileName = QFileInfo( partialFilePath ).fileName();
	
	foreach(XUPProjectItem* project, childrenProjects(true))
	{
		foreach(QString file, project->sourceFiles())
		{
			if (QFileInfo( file ).fileName() == searchFileName)
			{
				files << QFileInfo(project->filePath(file)); // absolute path
			}
		}
	}
	
	return files;
}

XUPProjectItem* XUPProjectItem::parentProject() const
{
	if ( mParentItem )
		return mParentItem->project();
	return const_cast<XUPProjectItem*>( this );
}

XUPProjectItem* XUPProjectItem::topLevelProject() const
{
	if ( mParentItem )
		return mParentItem->project()->topLevelProject();
	return const_cast<XUPProjectItem*>( this );
}

XUPProjectItem* XUPProjectItem::rootIncludeProject() const
{
	if ( mParentItem && mParentItem->type() == XUPItem::Function && mParentItem->attribute( "name" ).toLower() == "include" )
		return mParentItem->project()->rootIncludeProject();
	return const_cast<XUPProjectItem*>( this );
}

XUPProjectItemList XUPProjectItem::childrenProjects( bool recursive ) const
{
	XUPProjectModel* model = this->model();
	XUPProjectItem* thisProject = project();
	QMap<QString, XUPProjectItem*> projects;

	if ( model )
	{
		const QModelIndexList projectIndexes = model->match( thisProject->index(), XUPProjectModel::TypeRole, XUPItem::Project, -1, Qt::MatchExactly | Qt::MatchWrap | Qt::MatchRecursive );

		foreach ( const QModelIndex& index, projectIndexes )
		{
			XUPItem* item = static_cast<XUPItem*>( index.internalPointer() );
			XUPProjectItem* project = item->project();

			if ( recursive || project->parentProject() == thisProject )
			{
				projects[ project->fileName() ] = project;
			}
		}
	}

	return projects.values();
}

QString XUPProjectItem::variableDisplayText( const QString& variableName) const
{
	return variableName;
}

QString XUPProjectItem::variableDisplayIcon( const QString& variableName) const
{
	return variableName;
}

XUPItemList XUPProjectItem::getVariables( const XUPItem* root, const QString& variableName, bool recursive ) const
{
	XUPItemList variables;

	foreach(XUPItem* item, root->childrenList())
	{
		if ( XUPItem::Variable ==  item->type() &&
			 variableName == item->attribute( "name" ))
		{
			variables << item;
		}
		else if ( XUPItem::Project == item->type() )
		{
			if ( recursive )
			{
				XUPItem* pItem = item->parent();
				if ( pItem->type() == XUPItem::Function)
				{
					variables << getVariables( item, variableName );
				}
			}
		}
		else if ( recursive )
		{
			variables << getVariables( item, variableName );
		}
	}

	return variables;
}

XUPItem* XUPProjectItem::getVariable( const XUPItem* root, const QString& variableName) const
{
	foreach(XUPItem* item, root->childrenList())
	{
		if (XUPItem::Variable == item->type() &&
			item->attribute( "name" ) == variableName )
		{
			return item;
		}
	}

	return NULL;
}

QString XUPProjectItem::toString() const
{
	return mDocument.toString( 4 );
}

bool XUPProjectItem::open( const QString& fileName, const QString& codec )
{
	// get QFile
	QFile file( fileName );

	// check existence
	if ( !file.exists() )
	{
		topLevelProject()->setLastError( "file not exists" );
		return false;
	}

	// try open it for reading
	if ( !file.open( QIODevice::ReadOnly ) )
	{
		topLevelProject()->setLastError( "can't open file for reading" );
		return false;
	}

	// decode content
	QTextCodec* c = QTextCodec::codecForName( codec.toUtf8() );
	QString buffer = c->toUnicode( file.readAll() );

	// parse content
	QString errorMsg;
	int errorLine;
	int errorColumn;
	if ( !mDocument.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) )
	{
		topLevelProject()->setLastError( QString( "%1 on line: %2, column: %3" ).arg( errorMsg ).arg( errorLine ).arg( errorColumn ) );
		return false;
	}

	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
	if ( mDomElement.isNull() )
	{
		topLevelProject()->setLastError( "no project node" );
		return false;
	}

	// all is ok
	mCodec = codec;
	mFileName = fileName;
	topLevelProject()->setLastError( QString::null );
	file.close();

	return true;
//	return analyze( this );
}

bool XUPProjectItem::save()
{
	// try open file for writing
	QFile file( mFileName );

	if ( !file.open( QIODevice::WriteOnly ) )
	{
		return false;
	}

	// erase file content
	file.resize( 0 );

	// set xup version
	setAttribute( "version", XUP_VERSION );

	// encode content
	QTextCodec* codecObj = QTextCodec::codecForName( codec().toAscii ()  );
	QByteArray content = codecObj->fromUnicode( toString() );

	// write content
	bool result = file.write( content ) != -1;
	file.close();

	// set error message if needed
	if ( result )
	{
		topLevelProject()->setLastError( QString::null );
	}
	else
	{
		topLevelProject()->setLastError( tr( "Can't write content" ) );
	}

	return result;
}

QString XUPProjectItem::targetFilePath( bool, XUPProjectItem::TargetType )
{
	return QString::null;
}

QString XUPProjectItem::targetFilePath( const pCommandTargetExecution& execution )
{
	return targetFilePath( true, (XUPProjectItem::TargetType)execution.targetType );
}

QStringList XUPProjectItem::autoActivatePlugins() const
{
	return QStringList();
}

void XUPProjectItem::addCommand( pCommand& cmd, const QString& mnu )
{
	if ( cmd.isValid() )
	{
		cmd.setUserData( QVariant::fromValue( &mCommands ) );
		cmd.setProject( this );
		
		if ( cmd.workingDirectory().isEmpty() )
		{
			cmd.setWorkingDirectory( path() );
		}
		
		emit installCommandRequested( cmd, mnu );
		mCommands.insertMulti( mnu, cmd );
	}
}

void XUPProjectItem::installCommands()
{
}

void XUPProjectItem::uninstallCommands()
{
	foreach ( const pCommand& cmd, mCommands.values() )
		emit uninstallCommandRequested( cmd, mCommands.key( cmd ) );
	mCommands.clear();
}

QString XUPProjectItem::codec() const
{
	if (! mCodec.isNull())
		return mCodec;
	else
		return pMonkeyStudio::defaultCodec();
}

QString XUPProjectItem::sourceFileNameFilter() const
{
	const StringStringListList suffixes = sourceFileNamePatterns();
	QStringList allSuffixesList;
	QStringList suffixesList;
	
	foreach ( const PairStringStringList& pair, suffixes )
	{
		QString text = variableDisplayText( pair.first );
		suffixesList << QString( "%1 (%2)" ).arg( text ).arg( pair.second.join( " " ) );
		
		foreach ( const QString& suffixe, pair.second )
		{
			if ( !allSuffixesList.contains( suffixe ) )
			{
				allSuffixesList << suffixe;
			}
		}
	}
	
	suffixesList.prepend( tr( "All Files (*)" ) );
	
	if ( !allSuffixesList.isEmpty() )
	{
		suffixesList.prepend( tr( "All Supported Files (%2)" ).arg( allSuffixesList.join( " " ) ) );
	}
	
	return suffixesList.join( ";;" );

}

QStringList XUPProjectItem::filteredVariables() const
{
	return QStringList();
}
