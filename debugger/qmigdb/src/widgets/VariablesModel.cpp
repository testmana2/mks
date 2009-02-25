#include "VariablesModel.h"
#include "modeltest.h"

VariablesModel::VariablesModel (QGdb::Driver* driver)
  : QAbstractItemModel(),
  mDriver (driver)
{
	mArgumentsRoot.setText ("arguments");
	mLocalsRoot.setText ("locals");
	mWatchesRoot.setText ("watches");
	
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
	
	if (parent.internalPointer()) // nested item
	{
		return createIndex (row, column, ((QStandardItem*)parent.internalPointer())->child (row, column));
	}
	else // top level item
	{
		if (0 != column)
			return QModelIndex();
		
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
	
	QStandardItem* item = (QStandardItem*)index.internalPointer();
	QStandardItem* parent = item->parent();
	
	if (NULL == parent) // top level item
		return QModelIndex();
	
	
	int row = -1;
	int column = -1;
	
	QStandardItem* grandParent = parent->parent();
	if (NULL == grandParent) // parent is 'arguments', 'locals' or 'watches'
	{
		if (parent == &mArgumentsRoot)
		{
			row = 0;
			column = 0;
		}
		else if (parent == &mLocalsRoot)
		{
			row = 1;
			column = 0;
		}
		else if (parent == &mWatchesRoot)
		{
			row = 2;
			column = 0;
		}
	}
	else // parent is another regular item
	{
		// search parent item for get it's index
		for (int r = 0; r < grandParent->rowCount(); r++)
		{
			for (int c = 0; c < 2; c++)
			{
				if (grandParent->child (r, c) == parent)
				{
					row = r;
					column = c;
					break; // break nested for
				}
			}
			if (row != -1)
				break; // break top level for
		}
	}
	return createIndex (row, column, parent);
}

int VariablesModel::rowCount ( const QModelIndex & parent ) const
{
	if (parent.internalPointer()) // nested item
	{
		return ((QStandardItem*)parent.internalPointer())->rowCount();
	}
	else // top level item
	{
		return 3;
	}
}

int VariablesModel::columnCount ( const QModelIndex & parent ) const
{
	if (parent.internalPointer()) // nested item
	{
		qDebug () << "column count " << ((QStandardItem*)parent.internalPointer())->columnCount();
		return ((QStandardItem*)parent.internalPointer())->columnCount();
	}
	else // top level item
	{
		return 1;
	}
}

QVariant VariablesModel::data ( const QModelIndex & index, int role ) const
{
	if (index.internalPointer())
		return ((QStandardItem*)index.internalPointer())->data (role);
	
	return QVariant();
}

void VariablesModel::localsUpdated ()
{
	emit layoutAboutToBeChanged();
	mDriver->readLocals (&mLocalsRoot);
	emit layoutChanged();
	emit expand (index(1, 0));
}

void VariablesModel::argumentsUpdated ()
{
	emit layoutAboutToBeChanged();
	mDriver->readArguments (&mArgumentsRoot);
	emit layoutChanged();
	emit expand (index(0, 0));	
}
