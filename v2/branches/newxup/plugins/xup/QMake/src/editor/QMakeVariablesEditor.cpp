#include "QMakeVariablesEditor.h"

#include <XUPProjectItem.h>

QMakeVariablesEditor::QMakeVariablesEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent )
	: VariablesEditor( parent ), mPositiveValues( positive ), mNegativeValues( negative )
{
	/*setQuoteSpacedValuesString( "\"" );
	setQuoteSpacedValuesChecked( false );
	setQuoteSpacedValuesVisible( false );*/
}

QMakeVariablesEditor::~QMakeVariablesEditor()
{
}


XUPItemVariableEditorModel::FilterMode QMakeVariablesEditor::filterMode() const
{
	return XUPItemVariableEditorModel::Out;
}

bool QMakeVariablesEditor::isFriendlyDisplayText() const
{
	return false;
}

QStringList QMakeVariablesEditor::filteredVariables() const
{
	QStringList variables;
	
	variables
		<< "TEMPLATE"
		<< "TARGET"
		<< "DESTDIR"
		<< "DLLDESTDIR"
		<< "CONFIG"
		<< "QT"
		;
	
	if ( mProject ) {
		variables << mProject->documentFilters().fileVariables();
	}
	
	return variables;
}
