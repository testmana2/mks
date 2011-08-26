#include "QMakeVariablesEditor.h"
#include "UIQMakeEditor.h"

#include <XUPProjectItem.h>

QMakeVariablesEditor::QMakeVariablesEditor( QWidget* parent )
	: VariablesEditor( parent )
{
	/*setQuoteSpacedValuesString( "\"" );
	setQuoteSpacedValuesChecked( false );
	setQuoteSpacedValuesVisible( false );*/
	setDefaultOperator( "*=" );
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
	QStringList variables = UIQMakeEditor::handledVariables();
	
	if ( mProject ) {
		variables << mProject->documentFilters().fileVariables();
	}
	
	return variables;
}
