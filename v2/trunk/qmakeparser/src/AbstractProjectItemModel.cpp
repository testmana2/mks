#include "AbstractProjectItemModel.h"
//
#include <QFileInfo>
#include <QDir>
//
AbstractProjectItemModel::AbstractProjectItemModel( const QString& s, QObject* p )
	: QStandardItemModel( p ), mFilePath( s ), mIsOpen( false )
{
	setFilePath( s );
}
//
bool AbstractProjectItemModel::isOpen() const
{
	return mIsOpen;
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
//
void AbstractProjectItemModel::setFilePath( const QString& s )
{
	QString cfp = QFileInfo( s ).canonicalFilePath();
	if ( isOpen() || !QFile::exists( cfp ) || mFilePath == cfp )
		return;
	mFilePath = cfp;
	emit filePathChanged( mFilePath );
}
