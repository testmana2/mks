#ifndef UIPHPQTEDITOR_H
#define UIPHPQTEDITOR_H

#include "ui_UIPHPQtEditor.h"

#include <QMap>

class XUPProjectItem;
class XUPItem;

class UIPHPQtEditor : public QDialog, public Ui::UIPHPQtEditor
{
	Q_OBJECT
	
public:
	UIPHPQtEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UIPHPQtEditor();

protected:
	XUPProjectItem* mProject;
	QStringList mFileVariables;
	QMap<QString, QStringList> mValues;
	QMap<QString, QTreeWidgetItem*> mProjectFilesItems;
	QStringList mManagedVariables;
	QStringList mVariablesToRemove;
	
	QAction* aOthersValuesAddValue;
	QAction* aOthersValuesAddFile;
	QAction* aOthersValuesAddPath;
	QAction* aOthersValuesEditValue;
	QAction* aOthersValuesEditFile;
	QAction* aOthersValuesEditPath;
	
	void updateProjectFiles();
	void updateValuesEditorVariables();
	void updateValuesEditorValues( const QString& variable = QString::null );
	void init( XUPProjectItem* project );
	XUPItem* getUniqueVariableItem( const QString& variableName, bool create );

protected slots:
	void on_tbAddFile_clicked();
	void on_tbEditFile_clicked();
	void on_tbRemoveFile_clicked();
	
	// variables
	void on_lwOthersVariables_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	void on_tbOthersVariablesAdd_clicked();
	void on_tbOthersVariablesEdit_clicked();
	void on_tbOthersVariablesRemove_clicked();
	
	// values
	void on_lwOthersValues_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	void on_tbOthersValuesAdd_clicked();
	void on_tbOthersValuesAdd_triggered( QAction* action );
	void on_tbOthersValuesEdit_clicked();
	void on_tbOthersValuesEdit_triggered( QAction* action );
	void on_tbOthersValuesRemove_clicked();
	void on_tbOthersValuesClear_clicked();
	
	void accept();
};

#endif // UIPHPQTEDITOR_H
