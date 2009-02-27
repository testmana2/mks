#include "VariablesModelItem.h"

QGdb::VariablesModelItem::VariablesModelItem (const QString& name, const QString& value)
  : mName (name),
	mValue (value),
	mParent (NULL)
{
}

void QGdb::VariablesModelItem::setName(const QString& name)
{
	mName = name;
}

QString QGdb::VariablesModelItem::name() const
{
	return mName;
}

void QGdb::VariablesModelItem::setValue(const QString& value)
{
	mValue = value;
}

QString QGdb::VariablesModelItem::value() const
{
	return mValue;
}
		
int QGdb::VariablesModelItem::childCount() const
{
	return mChildren.size();
}

QGdb::VariablesModelItem* QGdb::VariablesModelItem::child (int index)
{
	return mChildren[index];
}

void QGdb::VariablesModelItem::addChild (VariablesModelItem* item)
{
	item->mParent = this;
	mChildren.append (item);
}

void QGdb::VariablesModelItem::insertChild (int index, VariablesModelItem* item)
{
	item->mParent = this;
	mChildren.insert (index, item);
}

void QGdb::VariablesModelItem::deleteAllChildren ()
{
	foreach (VariablesModelItem* child, mChildren)
	{
		child->deleteAllChildren();
		delete child;
	}
	mChildren.clear();
}

QGdb::VariablesModelItem* QGdb::VariablesModelItem::parent () const
{
	return mParent;
}