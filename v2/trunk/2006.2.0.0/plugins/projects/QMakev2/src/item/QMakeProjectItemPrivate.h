#ifndef QMAKEPROJECTITEMPRIVATE_H
#define QMAKEPROJECTITEMPRIVATE_H
//
#include <QVariant>
#include <QHash>
#include <QList>
//
class QMakeProjectItem;
//
class QMakeProjectItemPrivate : public QObject
{
	Q_OBJECT
	//
public:
	QMakeProjectItemPrivate();
	~QMakeProjectItemPrivate();
	void attach();
	void detach();
	//
	int mCount;
	Qt::ItemFlags mFlags;
	QHash<int, QVariant> mDatas;
	QList<QMakeProjectItem*> mChilds;
};
//
#endif // QMAKEPROJECTITEMPRIVATE_H
