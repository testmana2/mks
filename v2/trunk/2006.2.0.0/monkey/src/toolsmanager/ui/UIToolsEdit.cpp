#include "UIToolsEdit.h"
#include "Settings.h"
//
#include <QFileDialog>
#include <QWhatsThis>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileInfo>
#include <QUrl>
#include <QMimeData>
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
	leCaption->installEventFilter( this );
	tbFileIcon->installEventFilter( this );
	leFilePath->installEventFilter( this );
	leWorkingPath->installEventFilter( this );
	//
	QListWidgetItem* it;
	QSettings* s = Settings::current();
	int n = s->beginReadArray( "Tools" );
	for ( int i = 0; i < n; i++ )
	{
		s->setArrayIndex( i );
		it = new QListWidgetItem( lwTools );
		it->setData( idCaption, s->value( "Caption" ).toString() );
		it->setData( idFileIcon, s->value( "FileIcon" ).toString() );
		it->setData( idFilePath, s->value( "FilePath" ).toString() );
		it->setData( idWorkingPath, s->value( "WorkingPath" ).toString() );
		it->setIcon( QIcon( it->data( Qt::UserRole +1 ).toString() ) );
	}
	s->endArray();
}
//
void UIToolsEdit::closeEvent( QCloseEvent* e )
{
	if ( QMessageBox::question( this, tr( "Tools Editor..." ), tr( "You're about to discard all changes. Are you sure ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes ) == QMessageBox::No )
		e->ignore();
}
//
bool UIToolsEdit::eventFilter( QObject* o, QEvent* e )
{
	// accept drag enter event
	if ( e->type() == QEvent::DragEnter )
		e->accept();
	// if not a drop event, return
	if ( e->type() != QEvent::Drop )
		return QDialog::eventFilter( o, e );
	// got the event
	QDropEvent* d = static_cast<QDropEvent*>( e );
	// if no url drop it
	if ( !d || !d->mimeData()->hasUrls() )
		return QDialog::eventFilter( o, e );
	// if there is no current item selected, ask to create one
	QListWidgetItem* it = lwTools->currentItem();
	if ( !it && QMessageBox::question( this, tr( "Adding..." ), tr( "There is no current tool, do you want to add a new one ?" ), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No )
		return true; // true else the default drop event will be call
	else if ( !it )
		it = new QListWidgetItem( tr( "new Tool" ), lwTools );
	// get link info
	QFileInfo f( d->mimeData()->urls().at( 0 ).toLocalFile() );
	// drag for tbFileIcon
	if ( o == tbFileIcon )
	{
		if ( f.isFile() )
		{
			it->setData( idFileIcon, f.canonicalFilePath() );
			tbFileIcon->setIcon( QIcon( f.canonicalFilePath() ) );
			it->setIcon( tbFileIcon->icon() );
		}
	}
	// others
	else
	{
		if ( f.isFile() )
		{
			leCaption->setText( f.baseName() );
			it->setData( idCaption, leCaption->text() );
			leFilePath->setText( f.canonicalFilePath() );
			it->setData( idFilePath, leFilePath->text() );
			leWorkingPath->setText( f.canonicalPath() );
		}
		else if ( f.isDir() )
			leWorkingPath->setText( f.canonicalFilePath() );
		it->setData( idWorkingPath, leWorkingPath->text() );
	}
	// select the current item
	lwTools->clearSelection();
	lwTools->setCurrentItem( it );
	it->setSelected( true );
	// we finish
	return true;
}
//
void UIToolsEdit::on_lwTools_itemClicked( QListWidgetItem* i )
{
	if ( !i )
		return;
	leCaption->setText( i->text() );
	tbFileIcon->setIcon( i->icon() );
	leFilePath->setText( i->data( idFilePath ).toString() );
	leWorkingPath->setText( i->data( idWorkingPath ).toString() );
}
//
void UIToolsEdit::on_pbNew_clicked()
{
	QListWidgetItem* it = new QListWidgetItem( tr( "new Tool" ), lwTools );
	lwTools->clearSelection();
	lwTools->setCurrentItem( it );
	it->setSelected( true );
	on_lwTools_itemClicked( it );
}
//
void UIToolsEdit::on_pbDelete_clicked()
{
	delete lwTools->currentItem();
	if ( lwTools->count() )
	{
		lwTools->clearSelection();
		lwTools->setCurrentRow( 0 );
		lwTools->currentItem()->setSelected( true );
		on_lwTools_itemClicked( lwTools->currentItem() );
	}
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
	QString s = tr( "<b>Tools Editor ( Not Work Yet! )</b> give you the possibility to use variables<br><br>"
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
	if ( it )
		it->setData( idCaption, leCaption->text() );
}
//
void UIToolsEdit::on_tbFileIcon_clicked()
{
	QListWidgetItem* it = lwTools->currentItem();
	if ( !it )
		return;
	QString s = QFileDialog::getOpenFileName( this, tr( "Choose an icon for this tool" ), QString::null, tr( "Images (*.png *.xpm *.jpg)" ) );
	if ( s.isEmpty() )
		return;
	it->setData( idFileIcon, s );
	tbFileIcon->setIcon( QIcon( s ) );
	it->setIcon( tbFileIcon->icon() );
}
//
void UIToolsEdit::on_leFilePath_editingFinished()
{
	QListWidgetItem* it = lwTools->currentItem();
	if ( it )
		it->setData( idFilePath, leFilePath->text() );
}
//
void UIToolsEdit::on_tbFilePath_clicked()
{
	QListWidgetItem* it = lwTools->currentItem();
	if ( !it )
		return;
	QString s = QFileDialog::getOpenFileName( this, tr( "Choose the file to execute for this tool" ) );
	if ( s.isEmpty() )
		return;
	leFilePath->setText( s );
	leFilePath->setFocus();
}
//
void UIToolsEdit::on_tbUpdateWorkingPath_clicked()
{
	if ( !lwTools->currentItem())
		return;
	QFileInfo f( leFilePath->text() );
	if ( f.exists() && f.absolutePath() != leWorkingPath->text() )
	{
		leWorkingPath->setText( f.absolutePath() );
		leWorkingPath->setFocus();
	}
}
//
void UIToolsEdit::on_leWorkingPath_editingFinished()
{
	QListWidgetItem* it = lwTools->currentItem();
	if ( it )
		it->setData( idWorkingPath, leWorkingPath->text() );
}
//
void UIToolsEdit::on_tbWorkingPath_clicked()
{
	QListWidgetItem* it = lwTools->currentItem();
	if ( !it )
		return;
	QString s = QFileDialog::getOpenFileName( this, tr( "Choose the working path for this tool" ) );
	if ( s.isEmpty() )
		return;
	leWorkingPath->setText( s );
	leWorkingPath->setFocus();
}
//
void UIToolsEdit::accept()
{
	QSettings* s = Settings::current();
	s->beginWriteArray( "Tools" );
	for ( int i = 0; i < lwTools->count(); i++ )
	{
		s->setArrayIndex( i );
		s->setValue( "Caption", lwTools->item( i )->data( idCaption ).toString() );
		s->setValue( "FileIcon", lwTools->item( i )->data( idFileIcon ).toString() );
		s->setValue( "FilePath", lwTools->item( i )->data( idFilePath ).toString() );
		s->setValue( "WorkingPath", lwTools->item( i )->data( idWorkingPath ).toString() );
	}
	s->endArray();
	QDialog::accept();
}
