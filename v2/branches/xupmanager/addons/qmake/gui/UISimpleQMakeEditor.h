#ifndef UISIMPLEQMAKEEDITOR_H
#define UISIMPLEQMAKEEDITOR_H

#include "ui_UISimpleQMakeEditor.h"

#include <QMap>

class XUPProjectItem;

class UISimpleQMakeEditor : public QDialog, public Ui::UISimpleQMakeEditor
{
	Q_OBJECT
	
public:
	UISimpleQMakeEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UISimpleQMakeEditor();

protected:
	XUPProjectItem* mProject;
	QMap<QString, QString> mValues;
	
	void init( XUPProjectItem* project );

protected slots:
	void projectTypeChanged();
	void on_tbProjectTarget_clicked();
	void accept();
};

#endif // UISIMPLEQMAKEEDITOR_H
