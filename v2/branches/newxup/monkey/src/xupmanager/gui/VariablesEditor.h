#ifndef VARIABLESEDITOR_H
#define VARIABLESEDITOR_H

#include "MonkeyExport.h"

#include "ui_VariablesEditor.h"

class XUPProjectItem;
class XUPItem;
class StringItemModel;

class Q_MONKEY_EXPORT VariablesEditor : public QFrame, public Ui::VariablesEditor
{
	Q_OBJECT

public:
	VariablesEditor( QWidget* parent = 0 );
	virtual ~VariablesEditor();
	
	QStringList fileVariables() const;
	QStringList pathVariables() const;
	QStringList managedVariables() const;
	
	void init( XUPProjectItem* project );
	void finalize();

protected:
	QAction* aValuesAddValue;
	QAction* aValuesAddFile;
	QAction* aValuesAddPath;
	QAction* aValuesEditValue;
	QAction* aValuesEditFile;
	QAction* aValuesEditPath;
	XUPProjectItem* mProject;
	StringItemModel* mModel;
	
	XUPItem* variableItem( const QString& variableName, bool create );
	QString quotedString( const QString& string ) const;

protected slots:
	// variables
	void lvVariables_selectionModel_selectionChanged();
	void on_tbVariablesAdd_clicked();
	void on_tbVariablesEdit_clicked();
	
	// values
	void on_tbValuesAdd_clicked();
	void on_tbValuesAdd_triggered( QAction* action );
	void on_tbValuesEdit_clicked();
	void on_tbValuesEdit_triggered( QAction* action );
	void on_tbValuesClear_clicked();
};

#endif // VARIABLESEDITOR_H
