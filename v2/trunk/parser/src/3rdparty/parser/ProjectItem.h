#ifndef PROJECTITEM_H
#define PROJECTITEM_H
//
#include <QStandardItem>
//
class ProjectItem : public QObject,public QStandardItem
{
	Q_OBJECT
	Q_ENUMS( NodeRole NodeType )
public:
	enum NodeRole
	{
		TypeRole = Qt::UserRole, // the item type
		ValueRole,
		IndentRole,
		CommentRole,
		OperatorRole,
		AbsoluteFilePathRole,
		SortRole,
		SortRole2,
		FilterRole,
		ProjectRole
	};
	//
	enum NodeType
	{
		None = QStandardItem::UserType +1, // 1001
		Line,
		Scope,
		ScopeEnd,
		Variable,
		Value,
		File,
		Folder,
		Function,
		Project
	};
	//
	ProjectItem( ProjectItem::NodeType );
	//
	virtual int type() const;
	QList<ProjectItem*> children();
	//
private:
	ProjectItem::NodeType mType;
	//
};
//
#endif // PROJECTITEM_H