#include "VariablesModelItem.h"

QGdb::VariablesModelItem::VariablesModelItem (const QString& name, const QString& value)
  : mJustChanged (false),
	mName (name),
	mValue (value),
	mParent (NULL)
{
}

QGdb::VariablesModelItem* QGdb::VariablesModelItem::clone() const
{
	VariablesModelItem* cl = new VariablesModelItem (mName, mValue);
	foreach (VariablesModelItem* child, mChildren)
		cl->addChild (child->clone());  // recursion
	
	return cl;
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
	if (value != mValue)
	{
		mPrevValues << mValue;
		mValue = value;
		mJustChanged = true;
	}
}

QString QGdb::VariablesModelItem::value() const
{
	return mValue;
}
		
int QGdb::VariablesModelItem::childCount() const
{
	return mChildren.size();
}

QGdb::VariablesModelItem* QGdb::VariablesModelItem::child (int index) const
{
	return mChildren[index];
}

QGdb::VariablesModelItem* QGdb::VariablesModelItem::child (const QString& name) const
{
	foreach (VariablesModelItem* child, mChildren)
		if (child->name() == name)
			return child;
	
	return NULL;
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

bool QGdb::VariablesModelItem::hasChild (const QString& name) const
{
	foreach (VariablesModelItem* child, mChildren)
		if (child->name() == name)
			return true;
	
	return false;
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

void QGdb::VariablesModelItem::deleteChild (int index)
{
	Q_ASSERT (index < mChildren.size() && index >= 0);
	delete mChildren.takeAt (index);
}

QGdb::VariablesModelItem* QGdb::VariablesModelItem::parent () const
{
	return mParent;
}

QString QGdb::VariablesModelItem::prevValues() const
{
	return mPrevValues.join (" ,");
}

void QGdb::VariablesModelItem::clearJustChangedRecursive()
{
	mJustChanged = false;
	foreach (VariablesModelItem* child, mChildren)
		child->clearJustChangedRecursive();
}

bool QGdb::VariablesModelItem::isJustChanged() const
{
	return mJustChanged;
}
