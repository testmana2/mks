#include "UIXUPEditor.h"
#include "XUPProjectItem.h"
#include "XUPPageEditor.h"

#include "MainEditor.h"
#include "FilesEditor.h"
#include "VariablesEditor.h"
#include "CommandsEditor.h"

UIXUPEditor::UIXUPEditor( XUPProjectItem* project, QWidget* parent )
	: QDialog( parent )
{
	mProject = 0;
	
	setupUi( this );
	setWindowIcon( project->displayIcon() );
	setWindowTitle( tr( "Project Editor - %2" ).arg( project->displayText() ) );
	
	defaultSetup( project );
}

int UIXUPEditor::insertPage( int _index, XUPPageEditor* page )
{
	const int count = mPages.count();
	const int index = _index > 0 && _index < count ? _index : count;
	
	QListWidgetItem* item = new QListWidgetItem;
	item->setSizeHint( QSize( lwPages->width() -( lwPages->lineWidth() *2 ), 40 ) );
	item->setIcon( page->windowIcon() );
	item->setText( page->windowTitle() );
	
	mPages.insert( index, page );
	lwPages->insertItem( index, item );
	swPages->insertWidget( index, page );
	
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
	delete lwPages->item( index );
	delete swPages->widget( index );
}

QList<XUPPageEditor*> UIXUPEditor::pages() const
{
	return mPages;
}

void UIXUPEditor::setCurrentPage( int index )
{
	return lwPages->setCurrentRow( index );
}

int UIXUPEditor::currentPage() const
{
	return lwPages->currentRow();
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

UIXUPEditor::~UIXUPEditor()
{
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

void UIXUPEditor::accept()
{
	finalize();
	QDialog::accept();
}
