#include "QMakeFilesEditor.h"

QMakeFilesEditor::QMakeFilesEditor( QWidget* parent )
	: FilesEditor( parent )
{
	setQuoteSpacedValuesString( "\"" );
	setQuoteSpacedValuesChecked( true );
	setQuoteSpacedValuesVisible( false );
	//setDeleteRemovedFilesChecked( true );
	//setDeleteRemovedFilesVisible( false );
	setDefaultOperator( "*=" );
}

QMakeFilesEditor::~QMakeFilesEditor()
{
}
