#ifndef PACTIONGROUP_H
#define PACTIONGROUP_H

#include "MonkeyExport.h"

#include <QActionGroup>
#include <QPointer>

class QWidget;
class pAction;

class Q_MONKEY_EXPORT pActionGroup : public QActionGroup
{
	Q_OBJECT

public:
	pActionGroup( const QString&, const QString&, QWidget* );
	~pActionGroup();

	pAction* addAction( pAction* );

	static pActionGroup* defaultGroup();

private:
	static QPointer<pActionGroup> mDefaultGroup;

protected:
	virtual QAction* addAction( QAction* );
	virtual QAction* addAction( const QString& );
	virtual QAction* addAction( const QIcon&, const QString& );

};

#endif // PACTIONGROUP_H
