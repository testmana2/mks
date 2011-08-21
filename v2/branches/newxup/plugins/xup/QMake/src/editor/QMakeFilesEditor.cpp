#include "QMakeFilesEditor.h"

QMakeFilesEditor::QMakeFilesEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent )
	: FilesEditor( parent ), mPositiveValues( positive ), mNegativeValues( negative )
{
	setQuoteSpacedValuesChecked( true );
	setQuoteSpacedValuesString( "\"" );
	setQuoteSpacedValuesVisible( false );
	//setDeleteRemovedFilesChecked( true );
	//setDeleteRemovedFilesVisible( false );
}

QMakeFilesEditor::~QMakeFilesEditor()
{
}
