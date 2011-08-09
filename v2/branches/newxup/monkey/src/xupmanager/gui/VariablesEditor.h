#ifndef VARIABLESEDITOR_H
#define VARIABLESEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include <QModelIndex>

class Ui_VariablesEditor;

class XUPProjectItem;
class XUPItem;
class XUPItemVariableEditorModel;

class Q_MONKEY_EXPORT VariablesEditor : public XUPPageEditor
{
	Q_OBJECT

public:
	VariablesEditor( QWidget* parent = 0 );
	virtual ~VariablesEditor();
	
	bool isQuoteSpacedValuesVisible() const;
	bool isQuoteSpacedValuesEnabled() const;
	
	void setup( XUPProjectItem* project );
	void finalize();

public slots:
	void setQuoteSpacedValuesVisible( bool visible );
	void setQuoteSpacedValuesEnabled( bool enabled );

protected:
	Ui_VariablesEditor* ui;
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
	void lvVariables_selectionModel_selectionChanged();
	void on_tbVariablesAdd_clicked();
	void on_tbVariablesEdit_clicked();
	
	void lvValues_selectionModel_selectionChanged();
	void on_tbValuesAdd_clicked();
	void on_tbValuesAdd_triggered( QAction* action );
	void on_tbValuesEdit_clicked();
	void on_tbValuesEdit_triggered( QAction* action );
	void on_tbValuesClear_clicked();
};

#endif // VARIABLESEDITOR_H
