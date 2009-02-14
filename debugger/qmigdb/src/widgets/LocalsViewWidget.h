#ifndef LOCALSVIEWWIDGET_H
#define LOCALSVIEWWIDGET_H

#include "QGdbDriver.h"

#include <QTreeView>

class LocalsViewWidget : public QTreeView
{
	Q_OBJECT

public:
	LocalsViewWidget( QGdb::Driver* driver, QWidget* parent = 0 );
	virtual ~LocalsViewWidget();
};

#endif // CALLSTACKWIDGET_H
