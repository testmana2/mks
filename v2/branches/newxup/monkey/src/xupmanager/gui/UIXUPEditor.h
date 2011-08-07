#ifndef UISIMPLEQMAKEEDITOR_H
#define UISIMPLEQMAKEEDITOR_H

#include "ui_UIXUPEditor.h"

#include <QMap>

class XUPProjectItem;
class XUPItem;

class UIXUPEditor : public QDialog, public Ui::UIXUPEditor
{
	Q_OBJECT
	
public:
	UIXUPEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UIXUPEditor();
	
	void setVariableEditorVisible( bool visible );
	bool isVariableEditorVisible() const;

protected:
	XUPProjectItem* mProject;
	
	void updateMainFileComboBox( const QString& selectFile );
	void init( XUPProjectItem* project );

protected slots:
	void on_tbDynamicFolder_clicked();
	void accept();
};

#endif // UISIMPLEQMAKEEDITOR_H
