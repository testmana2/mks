#ifndef QMAKEPROJECTMODEL_H
#define QMAKEPROJECTMODEL_H
//
#include <QAbstractItemModel>
#include <QByteArray>
//
class QMakeProjectItem;
//
class QMakeProjectModel : public QAbstractItemModel
{
	Q_OBJECT
	friend class QMakeProjectItem;
	//
public:
	QMakeProjectModel( const QString& = QString::null, QObject* = 0 );
	~QMakeProjectModel();
	//
	bool openProject( const QString& );
	bool isOpen() const;
	virtual QModelIndex index( int, int, const QModelIndex& = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& ) const;
	virtual int rowCount( const QModelIndex& = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex& = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex&, int = Qt::DisplayRole ) const;
	virtual bool setData( const QModelIndex&, const QVariant&, int = Qt::EditRole );
	static Qt::ItemFlags defaultFlags();
	virtual Qt::ItemFlags flags( const QModelIndex& ) const;
	virtual QMakeProjectItem* itemFromIndex( const QModelIndex& ) const;
	virtual QModelIndex indexFromItem( const QMakeProjectItem* ) const;
	virtual QMakeProjectItem* row( int, QMakeProjectItem* = 0 ) const;
	virtual void appendRow( QMakeProjectItem*, QMakeProjectItem* = 0 );
	virtual void insertRow( int, QMakeProjectItem*, QMakeProjectItem* = 0 );
	virtual bool insertRows( int, int, const QModelIndex& = QModelIndex() ) ;
	virtual void removeRow( int, QMakeProjectItem* = 0 );
	virtual void removeRow( QMakeProjectItem*, QMakeProjectItem* = 0 );
	virtual bool removeRows( int, int, const QModelIndex& = QModelIndex() );
	virtual QMakeProjectItem* takeRow( int, QMakeProjectItem* = 0 );
	virtual QMakeProjectItem* takeRow( QMakeProjectItem*, QMakeProjectItem* = 0 );
	virtual QVariant headerData( int, Qt::Orientation, int ) const;
	//
protected:
	bool mOpen;
	QMakeProjectItem* mRootItem;
	//
};
//
#endif // QMAKEPROJECTMODEL_H
