#ifndef VARIABLES_MODEL_ITEM_H
#define VARIABLES_MODEL_ITEM_H

#include <QList>

#include "QGdb.h"

namespace QGdb
{
	class VariablesModelItem
	{
	public:
		VariablesModelItem (const QString& name, const QString& value);
		
		void setName(const QString& name);
		QString name() const;
		void setValue(const QString& value);
		QString value() const;
		
		int childCount() const;
		VariablesModelItem* child (int index);
		
		void addChild (VariablesModelItem* item);
		void insertChild (int index, VariablesModelItem* item);
		
		void deleteAllChildren ();
		
		VariablesModelItem* parent () const;
		
	protected:
		QString mName;
		QString mValue;
		QList<VariablesModelItem*> mChildren;
		VariablesModelItem* mParent;
	};
};

#endif // VARIABLES_MODEL_ITEM_H
