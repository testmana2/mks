#ifndef UISIMPLEQMAKEEDITOR_H
#define UISIMPLEQMAKEEDITOR_H

#include "ui_UIPyQtEditor.h"

#include <QMap>

class XUPProjectItem;
class XUPItem;

class UIPyQtEditor : public QDialog, public Ui::UIPyQtEditor
{
	Q_OBJECT
	
public:
	UIPyQtEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UIPyQtEditor();
	
protected:
	XUPProjectItem* mProject;
	
	void updateMainFileComboBox( const QString& selectFile );
	
	void init( XUPProjectItem* project );

protected slots:
	
	void accept();
};

#endif // UISIMPLEQMAKEEDITOR_H
