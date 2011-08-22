#ifndef QMAKEVARIABLESEDITOR_H
#define QMAKEVARIABLESEDITOR_H

#include <VariablesEditor.h>

#include "UIQMakeEditor.h"

class QMakeVariablesEditor : public VariablesEditor
{
	Q_OBJECT
	
public:
	QMakeVariablesEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent = 0 );
	virtual ~QMakeVariablesEditor();
	
	virtual XUPItemVariableEditorModel::FilterMode filterMode() const;
	virtual bool isFriendlyDisplayText() const;
	virtual QStringList filteredVariables() const;

protected:
	UIQMakeEditor::ProjectValues& mPositiveValues;
	UIQMakeEditor::ProjectValues& mNegativeValues;
};

#endif // QMAKEVARIABLESEDITOR_H
