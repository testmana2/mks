#include "FilesEditor.h"
#include "ui_FilesEditor.h"
#include "XUPItemVariableEditorModel.h"
#include "XUPProjectItem.h"

#include <QFileDialog>
#include <QMessageBox>

FilesEditor::FilesEditor( QWidget* parent )
	: QFrame( parent )
{
	ui = new Ui_FilesEditor;
	mProject = 0;
	mModel = new XUPItemVariableEditorModel( this );
	
	ui->setupUi( this );
	ui->tvVariables->setModel( mModel );
	
	connect( ui->tvVariables->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( tvVariables_selectionModel_selectionChanged() ) );
	connect( ui->cbQuoteEnabled, SIGNAL( clicked( bool ) ), mModel, SLOT( setQuoteValues( bool ) ) );
	connect( ui->cbQuote, SIGNAL( editTextChanged( const QString& ) ), mModel, SLOT( setQuoteString( const QString& ) ) );
	
	mModel->setQuoteString( ui->cbQuote->currentText() );
	mModel->setQuoteValues( ui->cbQuoteEnabled->isChecked() );
}

FilesEditor::~FilesEditor()
{
	delete ui;
}

void FilesEditor::setQuoteSpacedValuesVisible( bool visible )
{
	ui->wQuote->setVisible( visible );
}

bool FilesEditor::isQuoteSpacedValuesVisible() const
{
	return ui->wQuote->isVisible();
}

void FilesEditor::setQuoteSpacedValuesEnabled( bool enabled )
{
	ui->wQuote->setEnabled( enabled );
}

bool FilesEditor::isQuoteSpacedValuesEnabled() const
{
	return ui->wQuote->isEnabled();
}

void FilesEditor::setup( XUPProjectItem* project )
{
	mProject = project;
	mModel->setFilterMode( XUPItemVariableEditorModel::In );
	mModel->setFilteredVariables( project->documentFilters().fileVariables() );
	mModel->setRootItem( project );
	tvVariables_selectionModel_selectionChanged();
}

void FilesEditor::finalize()
{
	mModel->submit();
}

XUPItem* FilesEditor::variableItem( const QString& variableName, bool create )
{
	XUPItem* variableItem = mProject->getVariables( mProject, variableName, false, 0 ).value( 0 );
	
	// create it if needed
	if ( !variableItem && create ) {
		variableItem = mProject->addChild( XUPItem::Variable );
		variableItem->setAttribute( "name", variableName );
	}
	
	return variableItem;
}

QModelIndex FilesEditor::currentVariable() const
{
	const QModelIndex index = ui->tvVariables->selectionModel()->selectedIndexes().value( 0 );
	return index.isValid()
		? ( index.parent() == QModelIndex() ? index : index.parent() )
		: QModelIndex()
		;
}

QModelIndex FilesEditor::currentValue() const
{
	const QModelIndex index = ui->tvVariables->selectionModel()->selectedIndexes().value( 0 );
	return index.isValid()
		? ( index.parent() == QModelIndex() ? QModelIndex() : index )
		: QModelIndex()
		;
}

void FilesEditor::tvVariables_selectionModel_selectionChanged()
{
	const QModelIndex valueIndex = currentValue();
	ui->tbEdit->setEnabled( valueIndex.isValid() );
}

void FilesEditor::on_tbAdd_clicked()
{
	const DocumentFilterMap& filters = mProject->documentFilters();
	const QString filter = filters.sourceFileNameFilter();
	const QStringList files = QFileDialog::getOpenFileNames( QApplication::activeWindow(), tr( "Add files..." ), mProject->path(), filter, 0, QFileDialog::HideNameFilterDetails );
	QStringList unknownFiles;
	
	if ( !files.isEmpty() ) {
		foreach ( const QString& _file, files ) {
			const QString file = mProject->relativeFilePath( _file );
			const QString variableName = filters.fileNameVariable( file );
			
			if ( variableName.isEmpty() ) {
				unknownFiles << file;
				continue;
			}
			
			const QModelIndex variableIndex = mModel->addVariable( variableName );
			
			if ( !variableIndex.isValid() ) {
				Q_ASSERT( 0 );
				unknownFiles << file;
				continue;
			}
			
			const QModelIndex fileIndex = mModel->addValue( variableIndex, file );
			
			if ( fileIndex.isValid() ) {
				ui->tvVariables->setCurrentIndex( fileIndex );
				ui->tvVariables->scrollTo( fileIndex );
			}
			else {
				Q_ASSERT( 0 );
				unknownFiles << file;
				continue;
			}
		}
		
		if ( !unknownFiles.isEmpty() ) {
			QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "Some files have been ignored:\n%1" ).arg( unknownFiles.join( "\n" ) ) );
		}
	}
}

void FilesEditor::on_tbEdit_clicked()
{
	/*const QModelIndex variableIndex = currentVariable();
	const QModelIndex valueIndex = currentValue();
	
	if ( !variableIndex.isValid() || !valueIndex.isValid() ) {
		return;
	}
	
	const QString title = tr( "Edit a value..." );
	bool ok = true;
	QString value = valueIndex.data( Qt::DisplayRole ).toString();
	
	if ( value.isEmpty() || !ok ) {
		return;
	}
	
	if ( !mModel->setData( valueIndex, value, Qt::DisplayRole ) ) {
		QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This value already exists." ) );
	}*/
}
