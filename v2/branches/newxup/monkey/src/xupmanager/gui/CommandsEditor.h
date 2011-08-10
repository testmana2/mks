#ifndef COMMANDSEDITOR_H
#define COMMANDSEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include "ui_CommandsEditor.h"
#include "xupmanager/core/XUPProjectItemHelper.h"

class Q_MONKEY_EXPORT CommandsEditor : public XUPPageEditor, public Ui::CommandsEditor
{
	Q_OBJECT

public:
	CommandsEditor( QWidget* parent = 0 );
	virtual ~CommandsEditor();
	
	void setup( XUPProjectItem* project );
	void finalize();
	
	void setCommands( const MenuCommandListMap& commands );
	MenuCommandListMap commands() const;

protected:
	XUPProjectItem* mProject;
	MenuCommandListMap mCommands;
	QString mLastCommandMenu;

protected slots:
	void updateGui();
	void on_cbMenus_currentIndexChanged( int index );
	void on_tbCommandAdd_clicked();
	void on_tbCommandRemove_clicked();
	void on_tbCommandUp_clicked();
	void on_tbCommandDown_clicked();
	void on_lwCommands_itemSelectionChanged();
	void on_lwCommands_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
};

#endif // COMMANDSEDITOR_H
