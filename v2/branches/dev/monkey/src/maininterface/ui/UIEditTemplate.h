#ifndef UIEDITTEMPLATE_H
#define UIEDITTEMPLATE_H

#include "MonkeyExport.h"
#include "ui_UIEditTemplate.h"

class QTreeWidget;
class QTreeWidgetItem;

class Q_MONKEY_EXPORT UIEditTemplate : public QDialog, public Ui::UIEditTemplate
{
	Q_OBJECT

public:
	static void edit( QTreeWidget*, QTreeWidgetItem*, const QString& );

private:
	UIEditTemplate( QWidget*, QTreeWidget*, QTreeWidgetItem*, const QString& );
	~UIEditTemplate();

private slots:
	void on_tbIcon_clicked();
	void on_tbFilename_clicked();

};

#endif // UIEDITTEMPLATE_H
