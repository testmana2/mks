#include "QMakeProjectItemPrivate.h"
#include "QMakeProjectModel.h"
#include "QMakeProjectItem.h"
//
QMakeProjectItemPrivate::QMakeProjectItemPrivate()
{
	mCount = 1;
	mFlags = QMakeProjectModel::defaultFlags();
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
