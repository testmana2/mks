#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H
//
#include <QObject>
#include <QModelIndex>
#include <QPointer>
//
#include "AbstractProjectModel.h"
//
class QMakeProjectItemPrivate;
class QMakeProjectModel;
//
class QMakeProjectItem : public QObject
{
	Q_OBJECT
	friend class QMakeProjectModel;
	friend class QMakeProjectParser;
	//
public:
	QMakeProjectItem( AbstractProjectModel::NodeType = AbstractProjectModel::EmptyType, QMakeProjectItem* = 0, QMakeProjectItemPrivate* = 0 );
	~QMakeProjectItem();
	//
	QModelIndex index() const;
	virtual void clear();
	AbstractProjectModel::NodeType type() const;
	void setType( AbstractProjectModel::NodeType );
	Qt::ItemFlags flags() const;
	void setFlags( Qt::ItemFlags );
	QVariant data( int = Qt::DisplayRole ) const;
	void setData( const QVariant&, int = Qt::DisplayRole );
	void setData( QMakeProjectItemPrivate* );
	QMakeProjectItem* parent() const;
	void setParent( QMakeProjectItem* );
	QMakeProjectModel* model() const;
	int row() const;
	int column() const;
	int rowCount() const;
	int columnCount() const;
	QMakeProjectItem* row( int );
	QList<QMakeProjectItem*> rows() const;
	QMakeProjectItem* takeRow( int );
	QMakeProjectItem* takeRow( QMakeProjectItem* );
	void removeRow( int );
	void removeRow( QMakeProjectItem* );
	void appendRow( QMakeProjectItem* );
	void insertRow( int, QMakeProjectItem* );
	//
	bool swapRow( int, int );
	bool moveRowUp( int );
	bool moveRowDown( int );
	bool moveUp();
	bool moveDown();
	//
protected:
	void setPrivateModel( QMakeProjectModel* );
	void setPrivateData( const QVariant&, int = Qt::DisplayRole );
	bool insertPrivateRow( int, QMakeProjectItem* );
	bool removePrivateRow( int );
	//
	QPointer<QMakeProjectItem> mParent;
	QMakeProjectModel* mModel;
	QMakeProjectItemPrivate* d;
	//
};
//
#endif // QMAKEPROJECTITEM_H
