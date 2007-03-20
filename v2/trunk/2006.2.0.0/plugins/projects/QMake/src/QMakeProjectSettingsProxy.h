#ifndef QMAKEPROJECTSETTINGSPROXY_H
#define QMAKEPROJECTSETTINGSPROXY_H
//
#include "AbstractProjectProxy.h"
//
class QMakeProjectItemModel;
//
class QMakeProjectSettingsProxy : public AbstractProjectProxy
{
	Q_OBJECT
	//
public:
	QMakeProjectSettingsProxy( QMakeProjectItemModel* );
	//
private:
	QMakeProjectItemModel* mSource;

protected:
	virtual bool filterAcceptsRow( int, const QModelIndex& ) const;
	//
};
//
#endif // QMAKEPROJECTSETTINGSPROXY_H
