#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H
//
#include <QObject>
#include <QPointer>
#include <QModelIndex>
//
class QMakeProjectItemPrivate;
class QMakeProjectModel;
//
class QMakeProjectItem : public QObject
{
	Q_OBJECT
	Q_ENUMS( NodeRole NodeType )
	friend class QMakeProjectModel;
	//
public:
	enum NodeRole
	{
		FirstRole = Qt::UserRole, // first role
		TypeRole, // the item type
		OperatorRole, // the item operator
		ValueRole, // the item value
		CommentRole, // the item comment
		ProjectRole, // the project pointer
		AbsoluteFilePathRole, // the filepath of the current value if it s a file/path
		FileRole, // obsolete, tell if item is a file
		FolderRole, // oblsolete, tell if item is a folder
		LastRole // last role
	};
	//
	enum NodeType
	{
		FirstType = 0, // first type
		EmptyType, // empty line
		CommentType, // comment line
		NestedScopeType, // single line scope
		ScopeType, // multi line scope
		ScopeEndType, // end of a multi scope line
		VariableType, // variable
		ValueType, // value
		FunctionType, // function
		IncludeType, // include
		ProjectType, // project
		LastType // last type
	};
	//
	QMakeProjectItem( QMakeProjectItem::NodeType = QMakeProjectItem::EmptyType, QMakeProjectItem* = 0, QMakeProjectItemPrivate* = 0 );
	virtual ~QMakeProjectItem();
	//
	QModelIndex index() const;
	//
	virtual void clear();
	//
	QMakeProjectItem::NodeType type() const;
	void setType( QMakeProjectItem::NodeType );
	//
	Qt::ItemFlags flags() const;
	void setFlags( Qt::ItemFlags );
	//
	QVariant data( int = Qt::DisplayRole ) const;
	void setData( const QVariant&, int = Qt::DisplayRole );
	void setData( QMakeProjectItemPrivate* );
	//
	QMakeProjectItem* parent() const;
	void setParent( QMakeProjectItem* );
	//
	QMakeProjectModel* model() const;
	void setModel( QMakeProjectModel* );
	//
	int row() const;
	int column() const;
	//
	int rowCount() const;
	int columnCount() const;
	//
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
	void setPrivateData( const QVariant&, int = Qt::DisplayRole );
	bool removePrivateRow( int );
	QMakeProjectModel* mModel;
	QMakeProjectItem* mParent;
	QPointer<QMakeProjectItemPrivate> d;
	//
};
//
#endif // QMAKEPROJECTITEM_H
