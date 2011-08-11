#ifndef COMMANDSEDITOR_H
#define COMMANDSEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include "ui_CommandsEditor.h"
#include "xupmanager/core/XUPProjectItemHelper.h"

class CommandsEditorModel;

class Q_MONKEY_EXPORT CommandsEditor : public XUPPageEditor, public Ui::CommandsEditor
{
	Q_OBJECT

public:
	CommandsEditor( QWidget* parent = 0 );
	virtual ~CommandsEditor();
	
	void setup( XUPProjectItem* project );
	void finalize();

protected:
	CommandsEditorModel* mModel;
	XUPProjectItem* mProject;
	
	void setCommand( const QModelIndex& commandIndex );
	void getCommand( const QModelIndex& commandIndex );
	void updateState();

protected slots:
	void tvCommands_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
	void on_tbCommandAdd_clicked();
	void on_tbCommandUp_clicked();
	void on_tbCommandDown_clicked();
};

#endif // COMMANDSEDITOR_H
