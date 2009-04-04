#ifndef QCTAGSSENSEMEMBERSMODEL_H
#define QCTAGSSENSEMEMBERSMODEL_H

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPixmap>

#include "qCtagsSense.h"

class qCtagsSenseMembersThread;

class qCtagsSenseMembersModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	qCtagsSenseMembersModel( qCtagsSenseSQL* parent );
	virtual ~qCtagsSenseMembersModel();
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	
	static QPixmap entryPixmap( qCtagsSenseEntry* entry );

public slots:
	void populateFromFile( const QString& fileName );

protected:
	qCtagsSenseMembersThread* mThread;
	qCtagsSenseSQL* mSQL;
	qCtagsSenseEntry* mRootEntry;
	static QMap<QString, QPixmap> mPixmaps;

protected slots:
	void queryFinished( qCtagsSenseEntry* rootEntry );

signals:
	void populationFinished();
};

#endif // QCTAGSSENSEMEMBERSMODEL_H
