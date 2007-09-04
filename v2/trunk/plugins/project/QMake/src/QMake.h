#ifndef QMAKE_H
#define QMAKE_H

#include "ProjectPlugin.h"

class QMake : public ProjectPlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin ProjectPlugin )

public:
	QMake();
	virtual ~QMake();
	virtual bool setEnabled( bool );

	// the function that open a project
	virtual QStandardItem* openProject( const QString&, QStandardItem* = 0 );

protected slots:
	void saveSettings();
	void restoreSettings();

};

#endif // QMAKE_H
