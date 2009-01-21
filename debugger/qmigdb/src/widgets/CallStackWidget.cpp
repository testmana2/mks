#include "CallStackWidget.h"

#include <QDebug>

CallStackWidget::CallStackWidget( QGdbDriver* debugger, QWidget* parent )
	: QListWidget( parent )
{
	connect( debugger, SIGNAL( callStackUpdated( const QGdbDriver::CallStack& ) ), this, SLOT( update( const QGdbDriver::CallStack& ) ) );
}

CallStackWidget::~CallStackWidget()
{
}

void CallStackWidget::update( const QGdbDriver::CallStack& stack )
{
	clear();
	
	foreach ( const QGdbDriver::Frame& frame, stack )
	{
		QStringList lines;
		QStringList arguments;
		
		foreach ( const QGdbDriver::FunctionArgument& argument, frame.arguments )
		{
			arguments << QString( "%1 = %2" ).arg( argument.name ).arg( argument.value );
		}
		
		lines << QString( "#%1" ).arg( frame.level );
		lines << frame.function;
		
		if ( !arguments.isEmpty() )
		{
			lines << arguments.join( ", " ).prepend( "(" ).append( ")" );
		}
		
		if ( frame.from.isEmpty() )
		{
			lines << QString( "in %1" ).arg( frame.file );
		}
		else
		{
			lines << QString( "from %1" ).arg( frame.from );
		}
		
		if ( frame.from.isEmpty() )
		{
			lines << QString( ":%1" ).arg( frame.line );
		}
		
		QListWidgetItem* item = new QListWidgetItem( lines.join( " " ), this );
		item->setData( Qt::UserRole, QVariant::fromValue( frame ) );
	}
}
