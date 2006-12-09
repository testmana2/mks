#include "UIToolsEdit.h"
#include "Settings.h"
//
#include <QFileDialog>
#include <QWhatsThis>
//
QPointer<UIToolsEdit> UIToolsEdit::mSelf = 0L;
//
UIToolsEdit* UIToolsEdit::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new UIToolsEdit( p );
	return mSelf;
}
//
UIToolsEdit::UIToolsEdit( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	//
	QListWidgetItem* it;
	QSettings* s = Settings::current();
	int n = s->beginReadArray( "Tools" );
	for ( int i = 0; i < n; i++ )
	{
		s->setArrayIndex( i );
		it = new QListWidgetItem( s->value( "Caption" ).toString(), lwTools );
		it->setData( Qt::UserRole +1, s->value( "Icon" ).toString() );
		it->setIcon( QIcon( it->data( Qt::UserRole +1 ).toString() ) );
		it->setData( Qt::UserRole +2, s->value( "Command" ).toString() );
	}
	s->endArray();
}
//
void UIToolsEdit::on_lwTools_itemClicked( QListWidgetItem* i )
{
	if ( !i )
		return;
	leCaption->setText( i->text() );
	tbIcon->setIcon( i->icon() );
	lePath->setText( i->data( Qt::UserRole +2 ).toString() );
}
//
void UIToolsEdit::on_pbNew_clicked()
{
	lwTools->addItem( "new Tool" );
}
//
void UIToolsEdit::on_pbDelete_clicked()
{
	if ( lwTools->currentItem() )
		delete lwTools->currentItem();
}
//
void UIToolsEdit::on_pbUp_clicked()
{
	if ( !lwTools->currentItem() )
		return;
	int id = lwTools->currentRow();
	QListWidgetItem* it = lwTools->takeItem( id );
	lwTools->insertItem( id -1, it );
	lwTools->setCurrentRow( id -1 );
}
//
void UIToolsEdit::on_pbDown_clicked()
{
	if ( !lwTools->currentItem() )
		return;
	int id = lwTools->currentRow();
	QListWidgetItem* it = lwTools->takeItem( id );
	lwTools->insertItem( id +1, it );
	lwTools->setCurrentRow( id +1 );
}
//
void UIToolsEdit::on_tbHelp_clicked()
{
	QString s = tr( "<b>Tools Editor</b> give you the possibility to use variables<br><br>"
		"<b>%n</b> : Current project name<br>"
		"<b>%p</b> : Current project path<br>"
		"<b>%f</b> : Current project file path<br>"
		"<b>%d</b> : Selected file path ( In Project Box )<br>"
		"<b>%g</b> : Selected file file path ( In Project Box )" );
	QWhatsThis::showText( tbHelp->mapToGlobal( QPoint( 0, 0 ) ), s );
}
//
void UIToolsEdit::on_leCaption_editingFinished()
{
	QListWidgetItem* it = lwTools->currentItem();
	if ( !it )
		return;
	it->setText( leCaption->text() );
}
//
void UIToolsEdit::on_tbIcon_clicked()
{
	QListWidgetItem* it = lwTools->currentItem();
	if ( !it )
		return;
	QString s = QFileDialog::getOpenFileName( this, tr( "Choose an icon for this tool" ), QString::null, tr( "Images (*.png *.xpm *.jpg)" ) );
	if ( s.isEmpty() )
		return;
	it->setData( Qt::UserRole +1, s );
	tbIcon->setIcon( QIcon( s ) );
	it->setIcon( tbIcon->icon() );
}
//
void UIToolsEdit::on_lePath_editingFinished()
{
	QListWidgetItem* it = lwTools->currentItem();
	if ( !it )
		return;
	it->setData( Qt::UserRole +2, lePath->text() );
}
//
void UIToolsEdit::accept()
{
	QSettings* s = Settings::current();
	s->beginWriteArray( "Tools" );
	for ( int i = 0; i < lwTools->count(); i++ )
	{
		s->setArrayIndex( i );
		s->setValue( "Caption", lwTools->item( i )->text() );
		s->setValue( "Icon", lwTools->item( i )->data( Qt::UserRole +1 ).toString() );
		s->setValue( "Command", lwTools->item( i )->data( Qt::UserRole +2 ).toString() );
	}
	s->endArray();
	QDialog::accept();
}
