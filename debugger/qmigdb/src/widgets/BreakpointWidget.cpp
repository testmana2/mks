#include "BreakpointWidget.h"
#include "BreakpointEditorWidget.h"

#include <QDebug>

BreakpointWidget::BreakpointWidget( QGdb::Driver* driver, QWidget* parent )
	: QTreeWidget( parent )
{
	setColumnCount( 4 );
	setHeaderLabels( QStringList( tr( "Enabled" ) ) << tr( "Type" ) << tr( "Filename/Address" ) << tr( "Line" ) );
	
	connect( driver, SIGNAL( breakpointAdded( const QGdb::Breakpoint& ) ), this, SLOT( breakpointAdded( const QGdb::Breakpoint& ) ) );
	connect( driver, SIGNAL( breakpointRemoved( const QGdb::Breakpoint& ) ), this, SLOT( breakpointRemoved( const QGdb::Breakpoint& ) ) );
	connect( driver, SIGNAL( breakpointEdited( const QGdb::Breakpoint&, const QGdb::Breakpoint& ) ), this, SLOT( breakpointEdited( const QGdb::Breakpoint&, const QGdb::Breakpoint& ) ) );
	connect( driver, SIGNAL( breakpointsCleared() ), this, SLOT( breakpointsCleared() ) );
	
	connect( this, SIGNAL( break_breakpointEdited( const QGdb::Breakpoint&, const QGdb::Breakpoint& ) ), driver, SLOT( break_breakpointEdited( const QGdb::Breakpoint&, const QGdb::Breakpoint& ) ) );
	
	connect( this, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( self_itemDoubleClicked( QTreeWidgetItem*, int ) ) );
}

BreakpointWidget::~BreakpointWidget()
{
}

void BreakpointWidget::breakpointAdded( const QGdb::Breakpoint& breakpoint )
{
	QTreeWidgetItem* item = new QTreeWidgetItem( this );
	updateItem( item, breakpoint );
}

void BreakpointWidget::breakpointRemoved( const QGdb::Breakpoint& breakpoint )
{
	for ( int i = 0; i < topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* item = topLevelItem( i );
		QGdb::Breakpoint bp = item->data( 0, Qt::UserRole ).value<QGdb::Breakpoint>();
		
		if ( breakpoint.number == bp.number )
		{
			delete item;
			return;
		}
	}
}

void BreakpointWidget::breakpointEdited( const QGdb::Breakpoint& before, const QGdb::Breakpoint& after )
{
	for ( int i = 0; i < topLevelItemCount(); i++ )
	{
		QTreeWidgetItem* item = topLevelItem( i );
		QGdb::Breakpoint bp = item->data( 0, Qt::UserRole ).value<QGdb::Breakpoint>();
		
		if ( bp == before )
		{
			updateItem( item, after );
			return;
		}
	}
}

void BreakpointWidget::breakpointsCleared()
{
	clear();
}

void BreakpointWidget::self_itemDoubleClicked( QTreeWidgetItem* item, int column )
{
	Q_UNUSED( column );
	QGdb::Breakpoint bp = item->data( 0, Qt::UserRole ).value<QGdb::Breakpoint>();
	BreakpointEditorWidget bew( bp, this );
	
	if ( bew.exec() == QDialog::Accepted )
	{
		emit break_breakpointEdited( bp, bew.editedBreakpoint() );
	}
}

void BreakpointWidget::updateItem( QTreeWidgetItem* item, const QGdb::Breakpoint& breakpoint )
{
	item->setCheckState( 0, breakpoint.enabled ? Qt::Checked : Qt::Unchecked );
	item->setText( 1, breakpoint.stringType() );
	item->setText( 2, breakpoint.textMode() );
	item->setText( 3, QString::number( breakpoint.line ) );
	item->setData( 0, Qt::UserRole, QVariant::fromValue( breakpoint ) );
}
