#include "UIXUPEditor.h"
#include "ui_UIXUPEditor.h"
#include "XUPProjectItem.h"
#include "XUPPageEditor.h"

#include "MainEditor.h"
#include "FilesEditor.h"
#include "VariablesEditor.h"
#include "CommandsEditor.h"

UIXUPEditor::UIXUPEditor( XUPProjectItem* project, QWidget* parent )
	: QDialog( parent )
{
	ui = new Ui_UIXUPEditor;
	mProject = 0;
	
	ui->setupUi( this );
	ui->lwPages->setAttribute( Qt::WA_MacShowFocusRect, false );
	setWindowIcon( project->displayIcon() );
	setWindowTitle( tr( "Project Editor - %2" ).arg( project->displayText() ) );
	
	defaultSetup( project );
}

UIXUPEditor::~UIXUPEditor()
{
	delete ui;
}

int UIXUPEditor::insertPage( int _index, XUPPageEditor* page )
{
	const int count = mPages.count();
	const int index = _index > 0 && _index < count ? _index : count;
	
	QListWidgetItem* item = new QListWidgetItem;
	item->setSizeHint( QSize( ui->lwPages->width() -( ui->lwPages->lineWidth() *2 ) -( ui->lwPages->spacing() *2 ), 50 ) );
	item->setIcon( page->windowIcon() );
	item->setText( page->windowTitle() );
	
	mPages.insert( index, page );
	ui->lwPages->insertItem( index, item );
	ui->swPages->insertWidget( index, page );
	
	return index;
}

int UIXUPEditor::addPage( XUPPageEditor* page )
{
	return insertPage( -1, page );
}

void UIXUPEditor::addPages( const QList<XUPPageEditor*>& pages )
{
	foreach ( XUPPageEditor* page, pages ) {
		insertPage( -1, page );
	}
}

XUPPageEditor* UIXUPEditor::page( int index ) const
{
	return mPages.value( index );
}

void UIXUPEditor::removePage( int index )
{
	if ( index < 0 || index >= mPages.count() ) {
		return;
	}
	
	mPages.removeAt( index );
	delete ui->lwPages->item( index );
	delete ui->swPages->widget( index );
}

QList<XUPPageEditor*> UIXUPEditor::pages() const
{
	return mPages;
}

void UIXUPEditor::setCurrentPage( int index )
{
	ui->lwPages->setCurrentRow( index );
}

int UIXUPEditor::currentPage() const
{
	return ui->lwPages->currentRow();
}

void UIXUPEditor::defaultSetup( XUPProjectItem* project )
{
	QList<XUPPageEditor*> pages = QList<XUPPageEditor*>()
		<< new MainEditor
		<< new FilesEditor
		<< new VariablesEditor
		<< new CommandsEditor
		;
	
	addPages( pages );
	setup( project );
	setCurrentPage( 0 );
}

void UIXUPEditor::setup( XUPProjectItem* project )
{
	mProject = project;
	
	foreach ( XUPPageEditor* editor, mPages ) {
		editor->setup( project );
	}
}

void UIXUPEditor::finalize()
{
	foreach ( XUPPageEditor* editor, mPages ) {
		editor->finalize();
	}
}

void UIXUPEditor::on_lwPages_currentRowChanged( int row )
{
	QListWidgetItem* item = ui->lwPages->item( row );
	ui->lTitle->setText( item ? item->text() : QString::null );
	ui->lIcon->setPixmap( item ? item->icon().pixmap( QSize( 16, 16 ) ) : QPixmap() );
	ui->swPages->setCurrentIndex( row );
}

void UIXUPEditor::accept()
{
	finalize();
	QDialog::accept();
}
