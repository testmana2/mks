#include "BreakpointWidget.h"

#include <QDebug>

BreakpointWidget::BreakpointWidget( QGdbDriver* driver, QWidget* parent )
	: QTreeWidget( parent )
{
	setColumnCount( 4 );
	setHeaderLabels( QStringList( tr( "Enabled" ) ) << tr( "Type" ) << tr( "Filename/Address" ) << tr( "Line" ) );
	
	connect( driver, SIGNAL( breakpointAdded( const QGdbDriver::Breakpoint& ) ), this, SLOT( breakpointAdded( const QGdbDriver::Breakpoint& ) ) );
	connect( driver, SIGNAL( breakpointRemoved( const QGdbDriver::Breakpoint& ) ), this, SLOT( breakpointRemoved( const QGdbDriver::Breakpoint& ) ) );
	connect( driver, SIGNAL( breakpointUpdated( const QGdbDriver::Breakpoint& ) ), this, SLOT( breakpointUpdated( const QGdbDriver::Breakpoint& ) ) );
	connect( driver, SIGNAL( breakpointsCleared() ), this, SLOT( breakpointsCleared() ) );
}

BreakpointWidget::~BreakpointWidget()
{
}

void BreakpointWidget::breakpointAdded( const QGdbDriver::Breakpoint& breakpoint )
{
	QTreeWidgetItem* item = new QTreeWidgetItem( this );
	updateItem( item, breakpoint );
}

void BreakpointWidget::breakpointRemoved( const QGdbDriver::Breakpoint& breakpoint )
{
	for ( int i = 0; i < topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* item = topLevelItem( i );
		QGdbDriver::Breakpoint bp = item->data( 0, Qt::UserRole ).value<QGdbDriver::Breakpoint>();
		
		if ( breakpoint.number == bp.number )
		{
			delete item;
			return;
		}
	}
}

void BreakpointWidget::breakpointUpdated( const QGdbDriver::Breakpoint& breakpoint )
{
	for ( int i = 0; i < topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* item = topLevelItem( i );
		QGdbDriver::Breakpoint bp = item->data( 0, Qt::UserRole ).value<QGdbDriver::Breakpoint>();
		
		if ( breakpoint.number == bp.number )
		{
			updateItem( item, breakpoint );
			return;
		}
	}
}

void BreakpointWidget::breakpointsCleared()
{
	clear();
}

void BreakpointWidget::updateItem( QTreeWidgetItem* item, const QGdbDriver::Breakpoint& breakpoint )
{
	item->setCheckState( 0, breakpoint.enabled ? Qt::Checked : Qt::Unchecked );
	item->setText( 1, breakpoint.stringType() );
	item->setText( 2, breakpoint.textMode() );
	item->setText( 3, QString::number( breakpoint.line ) );
	item->setData( 0, Qt::UserRole, QVariant::fromValue( breakpoint ) );
}
