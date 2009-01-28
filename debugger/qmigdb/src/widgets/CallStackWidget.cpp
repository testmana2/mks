#include "CallStackWidget.h"

CallStackWidget::CallStackWidget( QGdbDriver* debugger, QWidget* parent )
	: QTreeWidget( parent )
{
	setColumnCount (4);
	QStringList headers;
	headers << "Nr" << "Function" << "Location" << "Line number";
	setHeaderLabels (headers);
	connect(this, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(onItemActivated (QTreeWidgetItem*)));
	
	// interface with debugger
	connect( debugger, SIGNAL( callStackUpdated( const QGdbDriver::CallStack&, int ) ), this, SLOT( update( const QGdbDriver::CallStack&, int ) ) );
}

CallStackWidget::~CallStackWidget()
{
}
void CallStackWidget::update( const QGdbDriver::CallStack& stack, int selectedLevel )
{
	clear();
	
	foreach ( const QGdbDriver::Frame& frame, stack )
	{
		QTreeWidgetItem* item = new QTreeWidgetItem (this);
		item->setText (0, QString::number (frame.level));
		
		// function name and arguments
		QStringList arguments;
		foreach ( const QGdbDriver::FunctionArgument& argument, frame.arguments )
		{
			arguments << QString( "%1 = %2" ).arg( argument.name ).arg( argument.value );
		}
		QString argString = arguments.join( ", " ).prepend( "(" ).append( ")" );
		item->setText (1, frame.function + argString);
		
		// library, file
		if ( frame.from.isEmpty() )
		{
			item->setText(2, QString( "in %1" ).arg( frame.file ));
		}
		else
		{
			item->setText(2, QString( "from %1:%2" ).arg( frame.from ).arg (frame.line));
		}
		
		// line
		item->setText (3, QString::number (frame.line));
		
		item->setData(0, Qt::UserRole, QVariant::fromValue( frame ) );
		
		if ( selectedLevel == frame.level )
		{
			setCurrentItem( item );
		}
	}
}

void CallStackWidget::onItemActivated (QTreeWidgetItem* item)
{
	QGdbDriver::Frame frame = item->data(0, Qt::UserRole).value<QGdbDriver::Frame>();
	emit frameSelected (frame.file, frame.line);
}
