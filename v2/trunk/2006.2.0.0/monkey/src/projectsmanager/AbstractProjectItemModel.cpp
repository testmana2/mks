#include "AbstractProjectItemModel.h"
//
#include <QHash>
#include <QFileInfo>
//
#include <qscilexer.h>
#include <qsciapis.h>
//
int AbstractProjectItemModel::mUniqueId = 0;
QHashProjects AbstractProjectItemModel::mProjectsList = QHashProjects();
//
AbstractProjectItemModel::AbstractProjectItemModel( const QString& s, AbstractProjectItemModel* p )
	: QStandardItemModel( p ), mId( mUniqueId++ ), mFilePath( s ), mIsOpen( false ),
	mIsModified( false ), mParentProject( p ), mLexer( 0 ), mAPIs( 0 )
{
	mProjectsList[ mId ] = this;
	setFilePath( s );
	// if parent project we take same lexer/api
	if ( mParentProject )
	{
		setLexer( p->lexer() );
		setAPIs( p->apis() );
	}
}
//
AbstractProjectItemModel::~AbstractProjectItemModel()
{
	// remove itself from projects list
	if ( mProjectsList.contains( mId ) )
		mProjectsList.remove( mId );
	// delete lexer and proxy if there is no parent
	if ( !mParentProject )
	{
		delete mAPIs;
		delete mLexer;
	}
}
//
int AbstractProjectItemModel::id() const
{
	return mId;
}
//
AbstractProjectItemModel* AbstractProjectItemModel::byId( int i )
{
	if ( !mProjectsList.contains( i ) )
		return 0;
	return mProjectsList.value( i );
}
//
QHashProjects AbstractProjectItemModel::all()
{
	return mProjectsList;
}
//
bool AbstractProjectItemModel::isOpen() const
{
	return mIsOpen;
}
//
bool AbstractProjectItemModel::isModified() const
{
	return mIsModified;
}
//
QsciLexer* AbstractProjectItemModel::lexer() const
{
	return mLexer;
}
// project apis
QsciAPIs* AbstractProjectItemModel::apis() const
{
	return mAPIs;
}
//
QString AbstractProjectItemModel::name() const
{
	return QFileInfo( mFilePath ).baseName();
}
//
QString AbstractProjectItemModel::path() const
{
	return QFileInfo( mFilePath ).path();
}
//
QString AbstractProjectItemModel::filePath() const
{
	return mFilePath;
}
//
QString AbstractProjectItemModel::filePath( const QString& s )
{
	return QDir::convertSeparators( QFileInfo( QString( "%1/%2" ).arg( path(), s ) ).canonicalFilePath() );
}
// get the parent project
AbstractProjectItemModel* AbstractProjectItemModel::parentProject() const
{
	return mParentProject;
}
// return all subprojects pointers recursively according to bool
QList<AbstractProjectItemModel*> AbstractProjectItemModel::subProjects( bool b ) const
{
	QList<AbstractProjectItemModel*> l = findChildren<AbstractProjectItemModel*>();
	if ( b )
		foreach ( AbstractProjectItemModel* p, l )
			l << p->subProjects( b );
	return l;
}
// get all files
QStringList AbstractProjectItemModel::getFiles( QDir d, const QStringList& f, bool b )
{
	QStringList l;
	if ( !d.exists() )
		return l;
	// looking recursive fodlers to load if needed
	if ( b )
	{
		foreach ( QString s, d.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) )
		{
			d.cd( s );
			l << getFiles( d, f, b );
			d.cdUp();
		}
	}
	// looking files to load
	foreach ( QString s, d.entryList( f, QDir::Files ) )
		l << QDir( path() ).relativeFilePath( d.absoluteFilePath( s ) );
	//
	return l;
}
//
void AbstractProjectItemModel::setFilePath( const QString& s )
{
	QString cfp = QFileInfo( s ).canonicalFilePath();
	if ( isOpen() || !QFile::exists( cfp ) || mFilePath == cfp )
		return;
	mFilePath = cfp;
	emit filePathChanged( mFilePath );
}
//
void AbstractProjectItemModel::setModified( bool b )
{
	if ( mIsModified == b )
		return;
	mIsModified = b;
	emit isModifiedChanged( mIsModified );
}
//
void AbstractProjectItemModel::setLexer( QsciLexer* l )
{
	delete mLexer;
	mLexer = l;
}
//
void AbstractProjectItemModel::setAPIs( QsciAPIs* a )
{
	delete mAPIs;
	mAPIs = a;
}
