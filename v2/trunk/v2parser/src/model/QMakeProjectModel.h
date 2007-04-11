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
	bool openProject();
	//
	virtual QModelIndex index( int, int, const QModelIndex& = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& ) const;
	virtual int rowCount( const QModelIndex& = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex& = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex&, int = Qt::DisplayRole ) const;
	virtual bool setData( const QModelIndex&, const QVariant&, int = Qt::EditRole );
	virtual Qt::ItemFlags flags( const QModelIndex& ) const;
	virtual QMakeProjectItem* itemFromIndex( const QModelIndex& ) const;
	virtual QModelIndex indexFromItem( QMakeProjectItem* ) const;
	virtual void insertRow( int, QMakeProjectItem* );
	virtual QMakeProjectItem* row( int ) const;
	virtual QVariant headerData( int, Qt::Orientation, int ) const;
	virtual void pReset();
	//
protected:
	virtual bool setupModelData( const QByteArray&, QMakeProjectItem* );
	bool mOpen;
	QString mProjectFilePath;
	QMakeProjectItem* mRootItem;
	//
};
//
#endif // QMAKEPROJECTMODEL_H
