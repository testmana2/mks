#include "VariablesEditor.h"
#include "ui_VariablesEditor.h"
#include "XUPItemVariableEditorModel.h"
#include "XUPProjectItem.h"

#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

VariablesEditor::VariablesEditor( QWidget* parent )
	: XUPPageEditor( parent )
{
	ui = new Ui_VariablesEditor;
	mProject = 0;
	mModel = new XUPItemVariableEditorModel( this );
	
	ui->setupUi( this );
	ui->lvVariables->setModel( mModel );
	ui->lvValues->setModel( mModel );
	
	// tbValuesAdd actions
	QMenu* addMenu = new QMenu( ui->tbValuesAdd );
	aValuesAddValue = addMenu->addAction( tr( "As Value..." ) );
	aValuesAddFile = addMenu->addAction( tr( "As File..." ) );
	aValuesAddPath = addMenu->addAction( tr( "As Path..." ) );
	ui->tbValuesAdd->setMenu( addMenu );
	
	// tbValuesEdit actions
	QMenu* editMenu = new QMenu( ui->tbValuesEdit );
	aValuesEditValue = editMenu->addAction( tr( "As Value..." ) );
	aValuesEditFile = editMenu->addAction( tr( "As File..." ) );
	aValuesEditPath = editMenu->addAction( tr( "As Path..." ) );
	ui->tbValuesEdit->setMenu( editMenu );
	
	connect( ui->lvVariables->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvVariables_selectionModel_selectionChanged() ) );
	connect( ui->lvValues->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvValues_selectionModel_selectionChanged() ) );
	connect( ui->cbQuoteEnabled, SIGNAL( clicked( bool ) ), mModel, SLOT( setQuoteValues( bool ) ) );
	connect( ui->cbQuote, SIGNAL( editTextChanged( const QString& ) ), mModel, SLOT( setQuoteString( const QString& ) ) );
	
	mModel->setQuoteString( ui->cbQuote->currentText() );
	mModel->setQuoteValues( ui->cbQuoteEnabled->isChecked() );
}

VariablesEditor::~VariablesEditor()
{
	delete ui;
}

void VariablesEditor::setQuoteSpacedValuesVisible( bool visible )
{
	ui->wQuote->setVisible( visible );
}

bool VariablesEditor::isQuoteSpacedValuesVisible() const
{
	return ui->wQuote->isVisible();
}

void VariablesEditor::setQuoteSpacedValuesEnabled( bool enabled )
{
	ui->wQuote->setEnabled( enabled );
}

bool VariablesEditor::isQuoteSpacedValuesEnabled() const
{
	return ui->wQuote->isEnabled();
}

void VariablesEditor::setup( XUPProjectItem* project )
{
	mProject = project;
	mModel->setFilteredVariables( project->documentFilters().fileVariables() );
	mModel->setRootItem( project );
	const QModelIndex index = mModel->index( 0, 0 );
	ui->lvVariables->setCurrentIndex( index );
	ui->lvVariables->scrollTo( index );
	lvVariables_selectionModel_selectionChanged();
}

void VariablesEditor::finalize()
{
	mModel->submit();
}

XUPItem* VariablesEditor::variableItem( const QString& variableName, bool create )
{
	XUPItem* variableItem = mProject->getVariables( mProject, variableName, false, 0 ).value( 0 );
	
	// create it if needed
	if ( !variableItem && create ) {
		variableItem = mProject->addChild( XUPItem::Variable );
		variableItem->setAttribute( "name", variableName );
	}
	
	return variableItem;
}

QModelIndex VariablesEditor::currentVariable() const
{
	const QModelIndex index = ui->lvVariables->selectionModel()->selectedIndexes().value( 0 );
	return ui->lvVariables->rootIndex() == index.parent() ? index : QModelIndex();
}

QModelIndex VariablesEditor::currentValue() const
{
	const QModelIndex index = ui->lvValues->selectionModel()->selectedIndexes().value( 0 );
	return ui->lvValues->rootIndex() == index.parent() ? index : QModelIndex();
}

void VariablesEditor::lvVariables_selectionModel_selectionChanged()
{
	const QModelIndex index = currentVariable();
	ui->tbVariablesEdit->setEnabled( index.isValid() );
	ui->gbValues->setEnabled( index.isValid() );
	ui->lvValues->setRootIndex( index );
	lvValues_selectionModel_selectionChanged();
}

void VariablesEditor::on_tbVariablesAdd_clicked()
{
	const QStringList variables = mModel->knownVariables();
	bool ok;
	
	const QString variable = QInputDialog::getItem( QApplication::activeWindow(), tr( "Add variable..." ), tr( "Select a variable name or enter a new one" ), variables, 0, true, &ok );
	
	if ( !variable.isEmpty() && ok ) {
		const QModelIndex index = mModel->addVariable( variable );
		
		if ( index.isValid() ) {
			ui->lvVariables->setCurrentIndex( index );
			ui->lvVariables->scrollTo( index );
		}
		else {
			QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This variable is filtered out." ) );
		}
	}
}

void VariablesEditor::on_tbVariablesEdit_clicked()
{
	const QModelIndex index = currentVariable();
	
	if ( !index.isValid() ) {
		return;
	}
	
	bool ok;
	QString variable = index.data( Qt::DisplayRole ).toString();
	variable = QInputDialog::getText( QApplication::activeWindow(), tr( "Edit variable..." ), tr( "Enter a new name for this variable" ), QLineEdit::Normal, variable, &ok );
	
	if ( !variable.isEmpty() && ok ) {
		if ( !mModel->setData( index, variable, Qt::DisplayRole ) ) {
			QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This variable exists or is filtered out." ) );
		}
	}
}

void VariablesEditor::lvValues_selectionModel_selectionChanged()
{
	const QModelIndex variableIndex = currentVariable();
	const QModelIndex valueIndex = currentValue();
	const int count = mModel->rowCount( variableIndex );
	ui->tbValuesEdit->setEnabled( valueIndex.isValid() );
	ui->tbValuesClear->setEnabled( variableIndex.isValid() && count > 0 );
}

void VariablesEditor::on_tbValuesAdd_clicked()
{
	on_tbValuesAdd_triggered( aValuesAddValue );
}

void VariablesEditor::on_tbValuesAdd_triggered( QAction* action )
{
	const QModelIndex variableIndex = currentVariable();
	
	if ( !variableIndex.isValid() ) {
		return;
	}
	
	const QString title = tr( "Add a value..." );
	bool ok = true;
	QString value;
	
	if ( action == aValuesAddValue ) {
		value = QInputDialog::getText( QApplication::activeWindow(), title, tr( "Enter the value :" ), QLineEdit::Normal, QString(), &ok );
		
		if ( !ok ) {
			value.clear();
		}
	}
	else if ( action == aValuesAddFile ) {
		value = QFileDialog::getOpenFileName( QApplication::activeWindow(), tr( "Choose a file" ), mProject->path() );
		
		if ( !value.isEmpty() ) {
			value = mProject->relativeFilePath( value );
		}
	}
	else if ( action == aValuesAddPath ) {
		value = QFileDialog::getExistingDirectory( QApplication::activeWindow(), tr( "Choose a path" ), mProject->path() );
		
		if ( !value.isEmpty() ) {
			value = mProject->relativeFilePath( value );
		}
	}
	
	if ( value.isEmpty() || !ok ) {
		return;
	}
	
	const QModelIndex index = mModel->addValue( variableIndex, value );
	ui->lvValues->setCurrentIndex( index );
	ui->lvValues->scrollTo( index );
}

void VariablesEditor::on_tbValuesEdit_clicked()
{
	on_tbValuesEdit_triggered( aValuesEditValue );
}

void VariablesEditor::on_tbValuesEdit_triggered( QAction* action )
{
	const QModelIndex variableIndex = currentVariable();
	const QModelIndex valueIndex = currentValue();
	
	if ( !variableIndex.isValid() || !valueIndex.isValid() ) {
		return;
	}
	
	const QString title = tr( "Edit a value..." );
	bool ok = true;
	QString value = valueIndex.data( Qt::DisplayRole ).toString();
	
	if ( action == aValuesEditValue ) {
		value = QInputDialog::getText( QApplication::activeWindow(), title, tr( "Edit the value :" ), QLineEdit::Normal, value, &ok );
		
		if ( !ok ) {
			value.clear();
		}
	}
	else if ( action == aValuesEditFile ) {
		value = QFileDialog::getOpenFileName( QApplication::activeWindow(), tr( "Choose a file" ), value );
		
		if ( !value.isEmpty() ) {
			value = mProject->relativeFilePath( value );
		}
	}
	else if ( action == aValuesEditPath ) {
		value = QFileDialog::getExistingDirectory( QApplication::activeWindow(), tr( "Choose a path" ), value );
		
		if ( !value.isEmpty() ) {
			value = mProject->relativeFilePath( value );
		}
	}
	
	if ( value.isEmpty() || !ok ) {
		return;
	}
	
	if ( !mModel->setData( valueIndex, value, Qt::DisplayRole ) ) {
		QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This value already exists." ) );
	}
}

void VariablesEditor::on_tbValuesClear_clicked()
{
	const QModelIndex variableIndex = currentVariable();
	
	if ( !variableIndex.isValid() ) {
		return;
	}
	
	if ( QMessageBox::question( QApplication::activeWindow(), tr( "Clear values..." ), tr( "Are you sure you want to clear these values ?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No ) {
		return;
	}
	
	for ( int i = 0; i < mModel->rowCount( variableIndex ); i++ ) {
		const QModelIndex index = variableIndex.child( i, 0 );
		mModel->setData( index, Qt::Unchecked, Qt::CheckStateRole );
	}
}
