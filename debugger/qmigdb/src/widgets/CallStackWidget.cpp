#include "CallStackWidget.h"

CallStackWidget::CallStackWidget( QGdbDriver* debugger, QWidget* parent )
	: QListWidget( parent )
{
	connect( debugger, SIGNAL( callStackUpdate( const QGdbDriver::CallStack& ) ), this, SLOT( update( const QGdbDriver::CallStack& ) ) );
}

CallStackWidget::~CallStackWidget()
{
}

void CallStackWidget::update( const QGdbDriver::CallStack& stack )
{
	clear();
	
	foreach ( const QGdbDriver::Frame& frame, stack )
	{
		QString text( "#%1 %2 (%3) at %4:%5" );
		QStringList arguments;
		
		foreach ( const QGdbDriver::FunctionArgument& argument, frame.arguments )
		{
			arguments << QString( "%1 = %2" ).arg( argument.name ).arg( argument.value );
		}
		
		text = text
			.arg( frame.level )
			.arg( frame.function )
			.arg( arguments.join( ", " ) )
			.arg( frame.file )
			.arg( frame.line );
		
		QListWidgetItem* item = new QListWidgetItem( text, this );
		item->setData( Qt::UserRole, QVariant::fromValue( frame ) );
	}
}
