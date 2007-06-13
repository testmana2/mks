#ifndef PACTION_H
#define PACTION_H

#include "MonkeyExport.h"

#include <QAction>

#include "pActionGroup.h"

class Q_MONKEY_EXPORT pAction : public QAction
{
	Q_OBJECT

public:
	pAction( const QString&, const QKeySequence&, pActionGroup* = pActionGroup::defaultGroup() );
	~pAction();

};

#endif // PACTION_H
