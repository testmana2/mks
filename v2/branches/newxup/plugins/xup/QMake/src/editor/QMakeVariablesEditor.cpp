#include "QMakeVariablesEditor.h"

QMakeVariablesEditor::QMakeVariablesEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent )
	: VariablesEditor( parent ), mPositiveValues( positive ), mNegativeValues( negative )
{
	setQuoteSpacedValuesChecked( true );
	setQuoteSpacedValuesString( "\"" );
	setQuoteSpacedValuesVisible( false );
}

QMakeVariablesEditor::~QMakeVariablesEditor()
{
}
