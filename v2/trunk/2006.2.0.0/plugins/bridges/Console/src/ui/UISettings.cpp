#include "UISettings.h"
#include "Settings.h"
//
#include <QTextCodec>
#include <QtAlgorithms>
//
UISettings::UISettings( QWidget* p )
	: QWidget( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	//
	QList<QByteArray> l =  QTextCodec::availableCodecs();
	qSort( l );
	foreach ( QByteArray ba, l )
		cbCodecs->addItem( QString::fromLocal8Bit( ba ) );
	//set value to current
	int i = cbCodecs->findText( Settings::current()->value( "Plugins/Console/Charset" ).toString().toLatin1() );
	if ( i == -1 )
		i = cbCodecs->findText( "System" );
	cbCodecs->setCurrentIndex ( i );   
}
//
void UISettings::on_bbDialog_clicked( QAbstractButton* )
{
	Settings::current()->setValue( "Plugins/Console/Charset", cbCodecs->currentText() );
	emit updateCodec();
}
