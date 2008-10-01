#ifndef XUPPROJECTMODEL_H
#define XUPPROJECTMODEL_H

#include <QAbstractItemModel>

class XUPProjectItem;

class XUPProjectModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	enum Roles { TypeRole = Qt::UserRole, TypeIdRole, ValueRole };
	XUPProjectModel( QObject* parent = 0 );
	virtual ~XUPProjectModel();

	// QAbstractItemModel reimplementation
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	
	// error handler
	void setLastError( const QString& error );
	QString lastError() const;
	
	// XUP Project members
	virtual bool open( XUPProjectItem* projectItem, const QString& fileName, const QString& encoding = QLatin1String( "UTF-8" ) );
	virtual void close();
	virtual bool save();
	
	// name of root project
	QString rootProjectName() const;
	QIcon rootProjectIcon() const;

protected:
	XUPProjectItem* mRootProject;
	QString mEncoding;
	QString mLastError;
	
	void handleIncludes();
};

Q_DECLARE_METATYPE( XUPProjectModel* )

#endif // XUPPROJECTMODEL_H
