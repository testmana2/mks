#ifndef STARTFORM_H
#define STARTFORM_H

#include <QWidget>

class QTreeView;

class StartForm : public QWidget
{
	Q_OBJECT
public:
	StartForm();
public slots:
	void slot_projectSettings();
private:
	QTreeView* treeView;
};

#endif //STARTFORM_H
