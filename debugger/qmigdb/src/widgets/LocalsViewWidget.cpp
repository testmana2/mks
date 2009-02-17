#include "LocalsViewWidget.h"

LocalsViewWidget::LocalsViewWidget( QGdb::Driver* driver, QWidget* parent )
	: QTreeView( parent )
{
	setHeaderHidden( true );
	
	setModel( driver->getLocalsModel() );
	
	connect( driver, SIGNAL( localsUpdated() ), this, SLOT( reset() ) );
}

LocalsViewWidget::~LocalsViewWidget()
{
}
