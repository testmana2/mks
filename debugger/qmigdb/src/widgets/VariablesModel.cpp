#include "VariablesModel.h"
#include "modeltest.h"

VariablesModel::VariablesModel (QGdb::Driver* driver)
  : QAbstractItemModel(),
	mArgumentsRoot ("arguments", ""),
	mLocalsRoot ("locals", ""),
	mWatchesRoot ("watches", ""),
    mDriver (driver)
{
	mArgumentsRoot.setName ("arguments");
	mLocalsRoot.setName ("locals");
	mWatchesRoot.setName ("watches");
	
	connect( driver, SIGNAL( localsUpdated() ), this, SLOT( localsUpdated() ) );
	connect( driver, SIGNAL( argumentsUpdated() ), this, SLOT( argumentsUpdated() ) );
	
	new ModelTest (this);
}

VariablesModel::~VariablesModel()
{
}

QModelIndex VariablesModel::index ( int row, int column, const QModelIndex & parent ) const
{
	if (column < 0 || column > 1) // invalid column
		return QModelIndex();
	
	QGdb::VariablesModelItem* parentItem = (QGdb::VariablesModelItem*)parent.internalPointer();
	if (parentItem) // nested item
	{
		if (row >= parentItem->childCount())
			return QModelIndex();
		return createIndex (row, column, parentItem->child (row));
	}
	else // top level item
	{
		if (row == 0)
			return createIndex (row, column, (void*)&mArgumentsRoot);
		else if (row == 1)
			return createIndex (row, column, (void*)&mLocalsRoot);
		else if (row == 2)
			return createIndex (row, column, (void*)&mWatchesRoot);
	}
	return QModelIndex(); // for avoid warning
}

QModelIndex VariablesModel::parent ( const QModelIndex & index ) const
{
	if (! index.isValid())
		return QModelIndex();
	
	QGdb::VariablesModelItem* item = (QGdb::VariablesModelItem*)index.internalPointer();
	QGdb::VariablesModelItem* parent = item->parent();
	
	if (NULL == parent) // top level item
		return QModelIndex();
	
	
	int row = -1;
	
	QGdb::VariablesModelItem* grandParent = parent->parent();
	if (NULL == grandParent) // parent is 'arguments', 'locals' or 'watches'
	{
		if (parent == &mArgumentsRoot)
		{
			row = 0;
		}
		else if (parent == &mLocalsRoot)
		{
			row = 1;
		}
		else if (parent == &mWatchesRoot)
		{
			row = 2;
		}
	}
	else // parent is another regular item
	{
		// search parent item for get it's index
		for (int r = 0; r < grandParent->childCount(); r++)
		{
			if (grandParent->child (r) == parent)
			{
				row = r;
				break; // break nested for
			}
			if (row != -1)
				break; // break top level for
		}
	}
	return createIndex (row, 0, parent);
}

int VariablesModel::rowCount ( const QModelIndex & parent ) const
{
	if (parent.isValid() && parent.column() != 0)
		return 0;
	
	if (parent.isValid() && parent.internalPointer()) // nested item
	{
		return ((QGdb::VariablesModelItem*)parent.internalPointer())->childCount();
	}
	else // top level item
	{
		return 3;
	}
}

int VariablesModel::columnCount ( const QModelIndex & ) const
{
	return 2;
}

QVariant VariablesModel::data ( const QModelIndex & index, int role ) const
{
	if (index.internalPointer())
	{
		switch (role)
		{
			case Qt::DisplayRole:			
				if (index.column() == 0)
					return ((QGdb::VariablesModelItem*)index.internalPointer())->name();
				else if (index.column() == 1)
					return ((QGdb::VariablesModelItem*)index.internalPointer())->value();
				else
					return QVariant();
			break;
			default:
				return QVariant();
			break;
		}
	}
	
	return QVariant();
}

void VariablesModel::mergeItems (QGdb::VariablesModelItem* dst, QGdb::VariablesModelItem* src)
{
	// Step 1:merge own item values
	Q_ASSERT (src->name() == dst->name());
	if (dst->value() != src->value())
	{
		dst->setValue (src->value());
		// TODO mark item as changed
	}
	
	// Step 2: remove from dst children, which disappeared (not present in src)
	for (int dstI = 0; dstI < dst->childCount(); dstI++)
	{
		if (! src->hasChild (dst->child(dstI)->name())) // child disappeared
		{
			dst->deleteChild (dstI);
			dstI--;
		}
	}
	
	// Step 2: update existing children of dst, or add new
	for (int srcI = 0; srcI < src->childCount(); srcI++)
	{
		QGdb::VariablesModelItem* srcChild = src->child (srcI); // update from this child
		QGdb::VariablesModelItem* dstChild = dst->child (srcChild->name()); // from to this child
		if (NULL != dstChild) // child exists, need update it
		{
			mergeItems (dstChild, srcChild); // recursive call
		}
		else // child not exists, need add it to dst item
		{
			dst->addChild (srcChild->clone());
		}
	}
}

void VariablesModel::localsUpdated ()
{
	emit layoutAboutToBeChanged();
	
	QGdb::VariablesModelItem newLocals ("locals", "");
	mDriver->readLocals (&newLocals);
	
	mergeItems (&mLocalsRoot, &newLocals);
	
	emit layoutChanged();
	
	emit expand (index(1, 0));
}

void VariablesModel::argumentsUpdated ()
{
	emit layoutAboutToBeChanged();
	
	QGdb::VariablesModelItem newArguments ("arguments", "");
	mDriver->readLocals (&newArguments);
	
	mergeItems (&mArgumentsRoot, &newArguments);
	
	mDriver->readArguments (&mArgumentsRoot);
	emit layoutChanged();
	emit expand (index(0, 0));	
}
