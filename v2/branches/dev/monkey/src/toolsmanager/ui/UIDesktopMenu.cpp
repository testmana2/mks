/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox
** Project   : UIDesktopMenu
** FileName  : UIDesktopMenu.cpp
** Date      : lun. août 13 19:14:07 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UIDesktopMenu.h"
#include "pToolsManager.h"
#include "pSettings.h"
#include "pMonkeyStudio.h"

#include <QTimer>

UIDesktopMenu::UIDesktopMenu( QWidget* w )
	: QDialog( w )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	pbLoading->setVisible( false );
	setProperty( "Populated", false );
}

UIDesktopMenu::~UIDesktopMenu()
{
}

void UIDesktopMenu::showEvent( QShowEvent* e )
{
	QDialog::showEvent( e );
	dbbButtons->button( QDialogButtonBox::Ok )->setDefault( false );
	QTimer::singleShot( 0, this, SLOT( populateList() ) );
}

void UIDesktopMenu::populateList()
{
	if ( !property( "Populated" ).toBool() )
	{
		// show progressbar
		pbLoading->setVisible( true );
		pbLoading->setRange( 0, mDesktopEntries.applications().count() );
		// clear tree
		lwLeft->clear();
		// load application
		foreach ( pDesktopFile df, mDesktopEntries.applications() )
		{
			QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
			QListWidgetItem* it = new QListWidgetItem( lwLeft );
			QString s;
			// caption
			it->setText( df.Name );
			// icon
			it->setIcon( QPixmap( mDesktopEntries.FindIcon( df.Icon, lwLeft->iconSize().width(), "Crystal SVG" ) ) );
			it->setData( Qt::UserRole +1, df.Icon );
			// tooltip
			it->setToolTip( QString( "<b>%1</b><br />%2" ).arg( df.GenericName.isEmpty() ? df.Name : df.GenericName, df.Comment.isEmpty() ? tr( "No Comment" ) : df.Comment ) );
			// filepath
			it->setStatusTip( df.FilePath );
			// categories
			it->setData( Qt::UserRole +2, df.Categories );
			// update progressbar
			pbLoading->setValue( pbLoading->value() +1 );
		}
		// hide progressbar
		pbLoading->setVisible( false );
		// restore to right added applications
		QStringList l;
		foreach ( pTool t, pToolsManager::tools( pToolsManager::ttDesktopEntry ) )
			l << t.FilePath;
		for ( int i = 0; i < lwLeft->count(); i++ )
		{
			QListWidgetItem* it = lwLeft->item( i );
			if ( l.contains( it->statusTip() ) )
				it->setSelected( true );
		}
		// simulate click to add items to right
		tbRight->click();
		// update property
		setProperty( "Populated", true );
	}
}

void UIDesktopMenu::on_leNameFilter_returnPressed()
{
	QList<QListWidgetItem*> l = lwLeft->findItems( leNameFilter->text(), Qt::MatchContains | Qt::MatchWrap );
	for ( int i = 0; i < lwLeft->count(); i++ )
	{
		// get item
		QListWidgetItem* it = lwLeft->item( i );
		//
		it->setHidden( !l.contains( it ) );
	}
}

void UIDesktopMenu::on_leCategoriesFilters_returnPressed()
{
	for ( int i = 0; i < lwLeft->count(); i++ )
	{
		// get item
		QListWidgetItem* it = lwLeft->item( i );
		// get item data
		QStringList l = it->data( Qt::UserRole +2 ).toStringList();
		// check if need to be visible according to filter
		bool b = leCategoriesFilters->text().isEmpty();
		if ( !b )
		{
			foreach ( QString s, leCategoriesFilters->text().split( ";" ) )
			{
				if ( l.contains( s, Qt::CaseInsensitive ) )
					b = true;
				if ( !b )
					break;
			}
		}
		// set item visibility
		it->setHidden( !b );
	}
}

void UIDesktopMenu::on_tbRight_clicked()
{
	foreach ( QListWidgetItem* it, lwLeft->selectedItems() )
		if ( !it->isHidden() )
			lwRight->addItem( lwLeft->takeItem( lwLeft->row( it ) ) );
}

void UIDesktopMenu::on_tbLeft_clicked()
{
	foreach ( QListWidgetItem* it, lwRight->selectedItems() )
		if ( !it->isHidden() )
			lwLeft->addItem( lwRight->takeItem( lwRight->row( it ) ) );
}

void UIDesktopMenu::on_pbUp_clicked()
{
	if ( lwRight->selectedItems().count() > 1 )
	{
		pMonkeyStudio::warning( tr( "Move Up..." ), tr( "Only one item can be move up, please select only one item." ) );
		return;
	}
	
	QListWidgetItem* it = lwRight->selectedItems().value( 0 );
	if ( !it || lwRight->row( it ) == 0 )
		return;
	int id = lwRight->row( it );
	it = lwRight->takeItem( id );
	lwRight->insertItem( id -1, it );
	lwRight->setCurrentRow( id -1 );
}

void UIDesktopMenu::on_pbDown_clicked()
{
	if ( lwRight->selectedItems().count() > 1 )
	{
		pMonkeyStudio::warning( tr( "Move Down..." ), tr( "Only one item can be move down, please select only one item." ) );
		return;
	}
	
	QListWidgetItem* it = lwRight->selectedItems().value( 0 );
	if ( !it || lwRight->row( it ) == lwRight->count() -1 )
		return;
	int id = lwRight->row( it );
	it = lwRight->takeItem( id );
	lwRight->insertItem( id +1, it );
	lwRight->setCurrentRow( id +1 );
}

void UIDesktopMenu::accept()
{
	// get desktop entry
	QList<pTool> l = pToolsManager::tools( pToolsManager::ttUserEntry );
	// get settings
	QSettings* s = pSettings::instance();
	// remove all tools entries
	s->remove( "Tools" );
	// begin array
	s->beginWriteArray( "Tools" );
	int i = 0;
	// write desktop entry
	for ( i = 0; i < lwRight->count(); i++ )
	{
		s->setArrayIndex( i );
		s->setValue( "Caption", lwRight->item( i )->text() );
		s->setValue( "FileIcon", mDesktopEntries.FindIcon( lwRight->item( i )->data( Qt::UserRole +1 ).toString(), 22, "Crystal SVG" ) );
		s->setValue( "FilePath", lwRight->item( i )->statusTip() );
		s->setValue( "WorkingPath", "" );
		s->setValue( "DesktopEntry", true );
	}
	// write user entry
	foreach ( pTool t, l )
	{
		s->setArrayIndex( i );
		s->setValue( "Caption", t.Caption );
		s->setValue( "FileIcon", t.FileIcon );
		s->setValue( "FilePath", t.FilePath );
		s->setValue( "WorkingPath", t.WorkingPath );
		s->setValue( "DesktopEntry", false );
		i++;
	}
	// end array
	s->endArray();
	// close dialog
	QDialog::accept();
}
