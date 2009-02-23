#include "VariablesModel.h"

#include "LocalsViewWidget.h"

LocalsViewWidget::LocalsViewWidget( QGdb::Driver* driver, QWidget* parent )
	: QTreeView( parent )
{
	setHeaderHidden( true );
	
	setModel (new VariablesModel (driver));
}

LocalsViewWidget::~LocalsViewWidget()
{
}
