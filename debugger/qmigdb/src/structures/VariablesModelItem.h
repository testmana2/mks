#ifndef VARIABLES_MODEL_ITEM_H
#define VARIABLES_MODEL_ITEM_H

#include <QList>
#include <QStringList>

#include "QGdb.h"

namespace QGdb
{
	class VariablesModelItem
	{
	public:
		VariablesModelItem (const QString& name, const QString& value);
		
		/* Create clone of item
		   NOTE: method allocates memory for clone. Caller have to carry about free this memory
		 */
		VariablesModelItem* clone() const;
		
		void setName(const QString& name);
		QString name() const;
		void setValue(const QString& value);
		QString value() const;
		
		int childCount() const;
		VariablesModelItem* child (int index) const;
		VariablesModelItem* child (const QString& name) const;
		
		void addChild (VariablesModelItem* item);
		void insertChild (int index, VariablesModelItem* item);
		
		bool hasChild (const QString& name) const;
		
		void deleteAllChildren ();
		void deleteChild (int index);
		
		VariablesModelItem* parent () const;
		
		QString prevValues() const;
		
		void clearJustChangedRecursive();
		bool isJustChanged() const;
	protected:
		bool mJustChanged;
		QString mName;
		QString mValue;
		QList<VariablesModelItem*> mChildren;
		VariablesModelItem* mParent;
		
		QStringList mPrevValues;
	};
};

#endif // VARIABLES_MODEL_ITEM_H
