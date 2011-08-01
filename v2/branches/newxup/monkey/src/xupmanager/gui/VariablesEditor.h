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
	
	inline QStringList& fileVariables() { return mFileVariables; }
	inline QStringList& pathVariables() { return mPathVariables; }
	inline QStringList& managedVariables() { return mManagedVariables; }
	
	void init( XUPProjectItem* project );
	void finalize();

protected:
	QAction* aOthersValuesAddValue;
	QAction* aOthersValuesAddFile;
	QAction* aOthersValuesAddPath;
	QAction* aOthersValuesEditValue;
	QAction* aOthersValuesEditFile;
	QAction* aOthersValuesEditPath;
	
	XUPProjectItem* mProject;
	QStringList mFileVariables;
	QStringList mPathVariables;
	QStringList mManagedVariables;
	StringItemModel* mModel;
	
	XUPItem* variableItem( const QString& variableName, bool create );

protected slots:
	// variables
	void lvVariables_selectionModel_selectionChanged();
	void on_tbOthersVariablesAdd_clicked();
	void on_tbOthersVariablesEdit_clicked();
	
	// values
	void on_tbOthersValuesAdd_clicked();
	void on_tbOthersValuesAdd_triggered( QAction* action );
	void on_tbOthersValuesEdit_clicked();
	void on_tbOthersValuesEdit_triggered( QAction* action );
	void on_tbOthersValuesClear_clicked();
};

#endif // VARIABLESEDITOR_H
