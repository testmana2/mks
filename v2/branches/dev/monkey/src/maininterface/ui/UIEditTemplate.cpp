#include "UIEditTemplate.h"
#include "pQScintilla.h"

#include <QTreeWidget>
#include <QImageReader>
#include <QFileDialog>

void UIEditTemplate::edit( QTreeWidget* t, QTreeWidgetItem* i, const QString& s )
{
	// create window
	UIEditTemplate d( t->window(), t, i, s );

	// add code template
	if ( d.exec() )
	{
		QTreeWidgetItem* it = t->findItems( d.leName->text(), Qt::MatchFixedString, 1 ).value( 0 );
		if ( !it || it == i )
		{
			if ( !i )
				i = new QTreeWidgetItem( t );
			i->setText( 0, d.cbLanguages->currentText() );
			i->setIcon( 0, d.tbIcon->icon() );
			i->setText( 1, d.leName->text() );
			i->setText( 2, d.leDescription->text() );
			// remember icon file
			i->setData( 0, Qt::UserRole, d.tbIcon->toolTip() );
			// remember template file
			const QString t = QString( s ).replace( "%HOME%", QDir::homePath() );
			QString f = d.leFilename->text();
			if ( f.contains( t ) )
				f.replace( t, "%TEMPLATE_PATH%" );
			i->setData( 0, Qt::UserRole +1, f );
		}
	}
}

UIEditTemplate::UIEditTemplate( QWidget* w, QTreeWidget* t, QTreeWidgetItem* i, const QString& s )
	: QDialog( w )
{
	setupUi( this );

	// fill combo with language
	QStringList l = pQScintilla::instance()->languages();
	l.sort();
	cbLanguages->addItems( l );

	// fill infos
	if ( i )
	{
		cbLanguages->setCurrentIndex( cbLanguages->findText( i->text( 0 ), Qt::MatchFixedString ) );
		leName->setText( i->text( 1 ) );
		leDescription->setText( i->text( 2 ) );
		tbIcon->setIcon( i->icon( 0 ) );
		tbIcon->setToolTip( i->data( 0, Qt::UserRole ).toString() );
		leFilename->setText( i->data( 0, Qt::UserRole +1 ).toString().replace( "%TEMPLATE_PATH%", QString( s ).replace( "%HOME%", QDir::homePath() ) ) );
	}
}

UIEditTemplate::~UIEditTemplate()
{
}

void UIEditTemplate::on_tbIcon_clicked()
{
	// get available image format
	QList<QByteArray> l = QImageReader::supportedImageFormats();

	// create filters list
	QString f;
	foreach ( QByteArray b, l )
		f.append( tr( "%1 Files (*.%2);;" ).arg( b.toUpper().constData() ).arg( b.constData() ) );
	if ( f.endsWith( ";;" ) )
		f.chop( 2 );

	// get a file
	QString s = QFileDialog::getOpenFileName( window(), tr( "Select an icon for this template type" ), tbIcon->toolTip(), f );

	// remember it
	if ( !s.isNull() )
	{
		tbIcon->setIcon( QIcon( s ) );
		tbIcon->setToolTip( s );
	}
}

void UIEditTemplate::on_tbFilename_clicked()
{
	// get a file
	QString s = QFileDialog::getOpenFileName( window(), tr( "Select a template file" ), leFilename->text() );

	// remember it
	if ( !s.isNull() )
		leFilename->setText( s );
}
