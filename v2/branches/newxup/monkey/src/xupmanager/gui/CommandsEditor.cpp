#include "CommandsEditor.h"
#include "CommandsEditorModel.h"
#include "MonkeyCore.h"

#include <pConsoleManager.h>
#include <pMenuBar.h>

#include <QDebug>

CommandsEditor::CommandsEditor( QWidget* parent )
	: XUPPageEditor( parent )
{
	mModel = new CommandsEditorModel( this );
	mProject = 0;
	
	setupUi( this );
	tvCommands->setModel( mModel );
	
	connect( tvCommands->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( tvCommands_selectionModel_selectionChanged( const QItemSelection&, const QItemSelection& ) ) );
}

CommandsEditor::~CommandsEditor()
{
}

void CommandsEditor::setup( XUPProjectItem* project )
{
	mProject = project;
	
	foreach ( const QString& parser, MonkeyCore::consoleManager()->parsersName() ) {
		QListWidgetItem* item = new QListWidgetItem( parser, lwCommandParsers );
		item->setCheckState( Qt::Unchecked );
	}
	
	mModel->setCommands( XUPProjectItemHelper::projectCommands( mProject ), MonkeyCore::menuBar() );
	tvCommands_selectionModel_selectionChanged( QItemSelection(), QItemSelection() );
}

void CommandsEditor::finalize()
{
	mModel->submit();
	XUPProjectItemHelper::setProjectCommands( mProject, mModel->commands() );
}

void CommandsEditor::setCommand( const QModelIndex& commandIndex )
{
	const bool isAction = commandIndex.isValid() && commandIndex.parent() != QModelIndex();
	
	if ( !isAction ) {
		return;
	}
	
	pCommand command = mModel->command( commandIndex );
	QStringList parsers;
	
	for ( int i = 0; i < lwCommandParsers->count(); i++ ) {
		QListWidgetItem* item = lwCommandParsers->item( i );
		
		if ( item->checkState() == Qt::Checked ) {
			parsers << item->text();
		}
	}
	
	command.setText( leCommandText->text() );
	command.setCommand( leCommandCommand->text() );
	command.setArguments( leCommandArguments->text() );
	command.setWorkingDirectory( leCommandWorkingDirectory->text() );
	command.setParsers( parsers );
	command.setSkipOnError( cbCommandSkipOnError->isChecked() );
	command.setTryAllParsers( cbCommandTryAll->isChecked() );
	
	mModel->setData( commandIndex, QVariant::fromValue( command ), Qt::EditRole );
}

void CommandsEditor::getCommand( const QModelIndex& commandIndex )
{
	const pCommand command = mModel->command( commandIndex );
	const QSet<QString> parsers = command.parsers().toSet();
	
	leCommandText->setText( command.text() );
	leCommandCommand->setText( command.command() );
	leCommandArguments->setText( command.arguments() );
	leCommandWorkingDirectory->setText( command.workingDirectory() );
	cbCommandSkipOnError->setChecked( command.skipOnError() );
	cbCommandTryAll->setChecked( command.tryAllParsers() );
	
	for ( int i = 0; i < lwCommandParsers->count(); i++ ) {
		QListWidgetItem* item = lwCommandParsers->item( i );
		item->setCheckState( parsers.contains( item->text() ) ? Qt::Checked : Qt::Unchecked );
	}
}

void CommandsEditor::updateState()
{
	const QModelIndex index = tvCommands->selectionModel()->selectedIndexes().value( 0 );
	const bool isAction = index.isValid() && index.parent() != QModelIndex();
	const int count = mModel->rowCount( index.parent() );
	
	tbCommandAdd->setEnabled( index.isValid() );
	tbCommandUp->setEnabled( isAction && index.row() > 0 && count > 1 );
	tbCommandDown->setEnabled( isAction && index.row() < count -1 && count > 1 );
	fEditor->setEnabled( isAction );
}

void CommandsEditor::tvCommands_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
	const QModelIndex oldIndex = deselected.indexes().value( 0 );
	const QModelIndex newIndex = selected.indexes().value( 0 );
	
	setCommand( oldIndex );
	getCommand( newIndex );
	updateState();
}

void CommandsEditor::on_tbCommandAdd_clicked()
{
	const QModelIndex index = tvCommands->selectionModel()->selectedIndexes().value( 0 );
	const bool isAction = index.isValid() && index.parent() != QModelIndex();
	const QModelIndex menuIndex = isAction ? index.parent() : index;
	const QModelIndex commandIndex = mModel->addCommand( menuIndex, pCommand( tr( "New command" ) ) );
	
	if ( commandIndex.isValid() ) {
		tvCommands->setCurrentIndex( commandIndex );
		tvCommands->scrollTo( commandIndex, QAbstractItemView::EnsureVisible );
	}
}

void CommandsEditor::on_tbCommandUp_clicked()
{
	const QModelIndex index = tvCommands->selectionModel()->selectedIndexes().value( 0 );
	mModel->swapCommand( index.parent(), index.row(), index.row() -1 );
	updateState();
}

void CommandsEditor::on_tbCommandDown_clicked()
{
	const QModelIndex index = tvCommands->selectionModel()->selectedIndexes().value( 0 );
	mModel->swapCommand( index.parent(), index.row(), index.row() +1 );
	updateState();
}
