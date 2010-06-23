#ifndef UISIMPLEQMAKEEDITOR_H
#define UISIMPLEQMAKEEDITOR_H

#include "ui_UIPHPEditor.h"

#include <QMap>

class XUPProjectItem;
class XUPItem;

class UIPHPEditor : public QDialog, public Ui::UIPHPEditor
{
	Q_OBJECT
	
public:
	UIPHPEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UIPHPEditor();
	
protected:
	XUPProjectItem* mProject;
	
	void updateMainFileComboBox( const QString& selectFile );
	
	void init( XUPProjectItem* project );

protected slots:
	
	void accept();
};

#endif // UISIMPLEQMAKEEDITOR_H
