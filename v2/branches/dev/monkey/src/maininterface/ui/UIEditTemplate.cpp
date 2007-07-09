#include "UIEditTemplate.h"
#include "pQScintilla.h"

#include <QTreeWidget>
#include <QImageReader>
#include <QFileDialog>
#include <QMessageBox>

void UIEditTemplate::edit( QTreeWidget* t, QTreeWidgetItem* i, const QString& s )
{
	// create dialog
	UIEditTemplate d( t->window(), t, i, s );

	// execute dialog
	d.exec();
}

UIEditTemplate::UIEditTemplate( QWidget* w, QTreeWidget* t, QTreeWidgetItem* i, const QString& s )
	: QDialog( w ), mTree( t ), mItem( i ), mString( s )
{
	setupUi( this );

	// fill combo with language
	QStringList l = pQScintilla::instance()->languages();
	l.sort();
	cbLanguages->addItems( l );

	// get correct full path
	mString.replace( "%HOME%", QDir::homePath() );

	// fill infos
	if ( i )
	{
		cbLanguages->setCurrentIndex( cbLanguages->findText( i->text( 0 ), Qt::MatchFixedString ) );
		leName->setText( i->text( 1 ) );
		leDescription->setText( i->text( 2 ) );
		tbIcon->setIcon( i->icon( 0 ) );
		tbIcon->setToolTip( i->data( 0, Qt::UserRole ).toString() );
		leFilename->setText( i->data( 0, Qt::UserRole +1 ).toString().replace( "%TEMPLATE_PATH%", mString ) );
	}

	// set focus on name lineedit
	leName->setFocus();
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

void UIEditTemplate::accept()
{
	// check if item already exists for same name
	QTreeWidgetItem* it = mTree->findItems( leName->text(), Qt::MatchFixedString, 1 ).value( 0 );

	// if not, create/update it
	if ( !it || it == mItem )
	{
		// create item if needed
		if ( !mItem )
			mItem = new QTreeWidgetItem( mTree );

		// fill item
		mItem->setText( 0, cbLanguages->currentText() );
		mItem->setIcon( 0, tbIcon->icon() );
		mItem->setText( 1, leName->text() );
		mItem->setText( 2, leDescription->text() );
		// remember icon file
		mItem->setData( 0, Qt::UserRole, tbIcon->toolTip() );
		QString f = leFilename->text();
		if ( f.contains( mString ) )
			f.replace( mString, "%TEMPLATE_PATH%" );
		mItem->setData( 0, Qt::UserRole +1, f );

		// set current item new one
		mTree->setCurrentItem( mItem );

		// accept
		QDialog::accept();
	}
	else
		QMessageBox::warning( window(), tr( "Warning..." ), tr( "This template name is already used." ) );
}
