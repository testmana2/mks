#ifndef PACTION_H
#define PACTION_H

#include "MonkeyExport.h"

#include <QAction>

#include "pActionManager.h"

class Q_MONKEY_EXPORT pAction : public QAction
{
	Q_OBJECT

public:
	pAction( const QString&, const QString&, const QKeySequence&, const QString& = pActionManager::globalGroup() );
	pAction( const QString&, const QIcon&, const QString&, const QKeySequence&, const QString& = pActionManager::globalGroup() );

	static const char* _DEFAULT_SHORTCUT_PROPERTY_;
	static const char* _GROUP_PROPERTY_;
	static const char* _SETTINGS_SCOPE_;

};

#endif // PACTION_H
