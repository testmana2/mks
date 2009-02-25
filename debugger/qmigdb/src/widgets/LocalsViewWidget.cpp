#include "VariablesModel.h"

#include "LocalsViewWidget.h"

LocalsViewWidget::LocalsViewWidget( QGdb::Driver* driver, QWidget* parent )
	: QTreeView( parent )
{
	//setHeaderHidden( true );
	VariablesModel* model = new VariablesModel (driver);
	connect (model, SIGNAL (expand (QModelIndex)), 
			 this, SLOT (expand (QModelIndex)));
	setModel (model);
}

LocalsViewWidget::~LocalsViewWidget()
{
}
