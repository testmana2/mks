#include <QTreeView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
//
#include "ProjectProxyModel.h"
#include "ProjectItemModel.h"
#include "ProjectItem.h"
#include "ProjectSettings.h"
#include "StartForm.h"

StartForm::StartForm() : QWidget()
{
	QString filePath;
	//filePath = QString::fromLocal8Bit("../example/demos.pro");
	//filePath = QString::fromLocal8Bit("../example/projects.pro");
	//filePath = QString::fromLocal8Bit("../example/arthurplugin.pro");
	//filePath = QString::fromLocal8Bit("../example/examples.pro");
	//filePath = QString::fromLocal8Bit("../example/monkey.pro");
	//filePath = QString::fromLocal8Bit("../example/src.pro");
	filePath = QString::fromLocal8Bit("../example/proManager.pro");
	//
	//subdirs check
	//filePath = QString::fromLocal8Bit("C:/Documents and Settings/Евгений/Рабочий стол/monkey/v2/trunk/2006.2.0.0/monkey.pro");
	//
	ProjectItemModel*  model = new ProjectItemModel();
	model->openFile(filePath);

	QVBoxLayout* lay = new QVBoxLayout(this);
	QCheckBox* chBox = new QCheckBox(0);
	QLabel* lab = new QLabel(0);
	QTextEdit* te = new QTextEdit(0);
	te->setReadOnly(true);

	chBox->setText("Filter On/Off");
	QPushButton* pb = new QPushButton("Write project");
	QPushButton* pb2 = new QPushButton("Settings");
	treeView = new QTreeView(0);
	QHBoxLayout* hLay = new QHBoxLayout();
	lay->addWidget(treeView);
	lay->addWidget(lab);
	hLay->addWidget(chBox);
	hLay->addWidget(pb2);
	hLay->addWidget(pb);
	lay->addLayout(hLay);
	lay->addWidget(te);

	ProjectProxyModel *proxyModel = new ProjectProxyModel(model);

	connect(chBox, SIGNAL(stateChanged(int)),
			proxyModel, SLOT(setFilter(int)));
	connect(treeView, SIGNAL(clicked( const QModelIndex &)),
			proxyModel, SLOT(currentProjectName( const QModelIndex&)));
	connect(proxyModel, SIGNAL(sig_curPro(const QString&)),
			lab, SLOT(setText(const QString&)) );
	connect(pb, SIGNAL(clicked()),
			model, SLOT(saveProject()));
	connect(model, SIGNAL(sig_writeText(const QString&)),
			te, SLOT(setPlainText(const QString&)));
	connect(pb2, SIGNAL(clicked()),
			this, SLOT(slot_projectSettings()));

	treeView->setModel(proxyModel);
}

void StartForm::slot_projectSettings()
{
	ProjectProxyModel* model = qobject_cast<ProjectProxyModel*>(treeView->model());
	ProjectSettings* ps = new ProjectSettings(model->project());
	ps->exec();
}
