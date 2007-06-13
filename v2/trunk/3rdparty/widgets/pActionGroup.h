#ifndef PACTIONGROUP_H
#define PACTIONGROUP_H

#include "MonkeyExport.h"

#include <QActionGroup>
#include <QPointer>

class QWidget;

class Q_MONKEY_EXPORT pActionGroup : public QActionGroup
{
	Q_OBJECT

public:
	pActionGroup( const QString&, QWidget* );
	~pActionGroup();

	static pActionGroup* defaultGroup();

private:
	static QPointer<pActionGroup> mDefaultGroup;

};

#endif // PACTIONGROUP_H
