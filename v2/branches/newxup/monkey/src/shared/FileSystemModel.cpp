#include "FileSystemModel.h"

QString FileSystemModel::MacOSXVolumes = "/Volumes";

FileSystemModel::FileSystemModel( QObject* parent )
	: QFileSystemModel( parent ), mShowHiddenFiles( false )
{
#ifdef Q_OS_MAC
	setFilter( filter() | QDir::CaseSensitive | QDir::Hidden );
#endif
}

QVariant FileSystemModel::data( const QModelIndex& index, int role ) const
{
	QVariant data = QFileSystemModel::data( index, role );

#ifdef Q_OS_MAC
	if ( index.column() == 0 && filePath( index ) == FileSystemModel::MacOSXVolumes ) {
		switch ( role ) {
			case Qt::DecorationRole:
				return myComputer( role ); 
			default:
				break;
		}
	}
	
	if ( ( filter() & QDir::Hidden ) && index.isValid() && !mShowHiddenFiles ) {
		if ( fileInfo( index ).isHidden() ) {
			if ( filePath( index ) != FileSystemModel::MacOSXVolumes ) {
				switch ( role ) {
					case Qt::SizeHintRole:
						return QSize( 0, 0 );
					default:
						return QVariant();
				}
			}
		}
	}
#endif
	
	return data;
}

QModelIndex FileSystemModel::index( int row, int column, const QModelIndex& parent ) const
{
	QModelIndex index = QFileSystemModel::index( row, column, parent );
	
/*#ifdef Q_OS_MAC
	if ( ( filter() & QDir::Hidden ) && index.isValid() && !mShowHiddenFiles ) {
		if ( fileInfo( index ).isHidden() ) {
			if ( filePath( index ) != FileSystemModel::MacOSXVolumes ) {
				index = QModelIndex();
			}
		}
	}
#endif*/
	
	return index;
}

bool FileSystemModel::showHiddenFiles() const
{
	return mShowHiddenFiles;
}

void FileSystemModel::setShowHiddenFiles( bool show )
{
#ifdef Q_OS_MAC
	emit layoutAboutToBeChanged();
	mShowHiddenFiles = show;
	emit layoutChanged();
#endif
}
