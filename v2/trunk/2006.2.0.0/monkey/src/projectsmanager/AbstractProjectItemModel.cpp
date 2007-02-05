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
AbstractProjectItemModel::AbstractProjectItemModel( const QString& s, QObject* p )
	: QStandardItemModel( p ), mFilePath( s ), mIsOpen( false )
{
	mId = mUniqueId;
	mUniqueId++;
	mProjectsList[ mId ] = this;
	setFilePath( s );
}
//
AbstractProjectItemModel::~AbstractProjectItemModel()
{
	if ( mProjectsList.contains( mId ) )
		mProjectsList.remove( mId );
	delete mAPIs;
	delete mLexer;
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
// get all files
QStringList AbstractProjectItemModel::recursiveFiles( QDir d )
{
	QStringList l;
	// looking fodlers to load
	foreach ( QString s, d.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) )
	{
		d.cd( s );
		l << recursiveFiles( d );
		d.cdUp();
	}
	// looking files to load
	foreach ( QString s, d.entryList( QDir::Files ) )
		l << d.absoluteFilePath( s );
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
	delete l;
	mLexer = l;
}
//
void AbstractProjectItemModel::setAPIs( QsciAPIs* a )
{
	delete mAPIs;
	mAPIs = a;
}
