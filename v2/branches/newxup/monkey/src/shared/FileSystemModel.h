#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include "MonkeyCore.h"

#include <QFileSystemModel>

class Q_MONKEY_EXPORT FileSystemModel : public QFileSystemModel
{
	Q_OBJECT
	
public:
	FileSystemModel( QObject* parent = 0 );
	
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	
	bool showHiddenFiles() const;

public slots:
	void setShowHiddenFiles( bool show );

protected:
	static QString MacOSXVolumes;
	bool mShowHiddenFiles;
};

#endif // FILESYSTEMMODEL_H
