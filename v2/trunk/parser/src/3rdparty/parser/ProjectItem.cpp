#include "ProjectItem.h"
//
ProjectItem::ProjectItem( ProjectItem::NodeType t )
		: mType( t )
{
	setData( t, ProjectItem::TypeRole );
}
//
int ProjectItem::type() const
{
	return mType;
}
//
QList <ProjectItem*> ProjectItem::children()
{
	QList <ProjectItem*> list;
	int i = 0;
	ProjectItem* item = (ProjectItem*)this->child(i);
	while (item != 0)
	{
		list<<item;
		item = (ProjectItem*)this->child(++i);
	}
	return list;
}