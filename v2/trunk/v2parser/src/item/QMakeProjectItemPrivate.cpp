#include "QMakeProjectItemPrivate.h"
#include "QMakeProjectItem.h"
//
QMakeProjectItemPrivate::QMakeProjectItemPrivate()
{
	mCount = 1;
	mFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
//
QMakeProjectItemPrivate::~QMakeProjectItemPrivate()
{
	mDatas.clear();
	qDeleteAll( mChilds );
	mChilds.clear();
}
//
void QMakeProjectItemPrivate::attach()
{
	mCount++;
}
//
void QMakeProjectItemPrivate::detach()
{ 
	mCount--; 
	if( !mCount ) 
		delete this;
}
