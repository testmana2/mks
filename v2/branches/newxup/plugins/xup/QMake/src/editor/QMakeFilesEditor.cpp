#include "QMakeFilesEditor.h"

QMakeFilesEditor::QMakeFilesEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent )
	: FilesEditor( parent ), mPositiveValues( positive ), mNegativeValues( negative )
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
