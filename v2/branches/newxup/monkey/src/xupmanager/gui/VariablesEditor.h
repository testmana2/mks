#ifndef VARIABLESEDITOR_H
#define VARIABLESEDITOR_H

#include "MonkeyExport.h"

#include "ui_VariablesEditor.h"

class XUPProjectItem;
class XUPItem;
class XUPItemVariableEditorModel;

class Q_MONKEY_EXPORT VariablesEditor : public QFrame, public Ui::VariablesEditor
{
	Q_OBJECT

public:
	VariablesEditor( QWidget* parent = 0 );
	
	void setup( XUPProjectItem* project );
	void finalize();

protected:
	QAction* aValuesAddValue;
	QAction* aValuesAddFile;
	QAction* aValuesAddPath;
	QAction* aValuesEditValue;
	QAction* aValuesEditFile;
	QAction* aValuesEditPath;
	XUPProjectItem* mProject;
	XUPItemVariableEditorModel* mModel;
	
	XUPItem* variableItem( const QString& variableName, bool create );
	QModelIndex currentVariable() const;
	QModelIndex currentValue() const;

protected slots:
	// variables
	void lvVariables_selectionModel_selectionChanged();
	void on_tbVariablesAdd_clicked();
	void on_tbVariablesEdit_clicked();
	
	// values
	void lvValues_selectionModel_selectionChanged();
	void on_tbValuesAdd_clicked();
	void on_tbValuesAdd_triggered( QAction* action );
	void on_tbValuesEdit_clicked();
	void on_tbValuesEdit_triggered( QAction* action );
	void on_tbValuesClear_clicked();
};

#endif // VARIABLESEDITOR_H
