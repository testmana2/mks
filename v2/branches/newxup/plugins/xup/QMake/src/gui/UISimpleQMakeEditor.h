#ifndef UISIMPLEQMAKEEDITOR_H
#define UISIMPLEQMAKEEDITOR_H

#include "ui_UISimpleQMakeEditor.h"
#include "../QtVersionManager.h"

#include <QMap>

class QMakeProjectItem;
class XUPProjectItem;
class XUPItem;

class UISimpleQMakeEditor : public QDialog, public Ui::UISimpleQMakeEditor
{
	Q_OBJECT
	
public:
	UISimpleQMakeEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UISimpleQMakeEditor();

protected:
	QMakeProjectItem* mProject;
	QStringList mConfigGui;
	QtVersion mQtVersion;
	QMap<QString, QString> mValues;
	QStringList mManagedVariables;
	
	QStringList mVariablesToRemove;
	
	QAction* aOthersValuesAddValue;
	QAction* aOthersValuesAddFile;
	QAction* aOthersValuesAddPath;
	QAction* aOthersValuesEditValue;
	QAction* aOthersValuesEditFile;
	QAction* aOthersValuesEditPath;
	
	void updateValuesEditorVariables();
	void updateValuesEditorValues( const QString& variable = QString::null );
	void init( XUPProjectItem* project );
	XUPItem* getUniqueVariableItem( const QString& variableName, bool create );

protected slots:
	void projectTypeChanged();
	void on_tbProjectTarget_clicked();
	void modules_itemSelectionChanged();
	
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

#endif // UISIMPLEQMAKEEDITOR_H
