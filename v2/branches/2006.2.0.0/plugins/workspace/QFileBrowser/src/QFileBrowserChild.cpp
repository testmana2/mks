#include "QFileBrowserChild.h"
#include "Workspace.h"

#include <QDockWidget>
#include <QPointer>
#include <QListView>
#include <QLabel>
#include <QWidget>
#include <QDirModel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDirModel>
#include <QMouseEvent>
#include <QMessageBox>
#include <QModelIndex>
#include <QString>
#include <QComboBox>
#include <QFileInfoList>
#include <QDir>
#include <QShowEvent>

QPointer<QFileBrowserChild> QFileBrowserChild::mSelf = 0L;

QFileBrowserChild* QFileBrowserChild::self( Workspace* w )
{
	if ( !mSelf )
		mSelf = new QFileBrowserChild( w );
	return mSelf;
}

QFileBrowserChild::QFileBrowserChild( Workspace* w)
{
	Q_ASSERT( w );
	mWorkspace = w;
	mDock = new QDockWidget();
	mLayout = new QVBoxLayout();
	mListView = new QListView();
	mModel = new QDirModel();
	mComboBox = new QComboBox();
	mWidget = new QWidget();
	mModel->setFilter(QDir::AllEntries);
	mListView->setModel(mModel);
	mComboBox->setModel(mModel);
	mListView->setRootIndex(mModel->index(mComboBox->currentText()));
	mDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	mListView->resize(mDock->width(), mDock->height());
	mListView->setSelectionMode(QAbstractItemView::SingleSelection);
	mLayout->addWidget(mComboBox);
	mLayout->addWidget(mListView);
	mWidget->setLayout(mLayout);
	mDock->setWidget(mWidget);
	connect(mListView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(clickReaction(const QModelIndex&)));
	connect(mComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setDrivers(const QString&)));
}

void QFileBrowserChild::clickReaction(const QModelIndex& index)
{
	if(mModel->isDir(index))
	{
		mListView->setRootIndex(index);
	}
	else
	{
		mDir = new QDir(mModel->filePath(index));
		mWorkspace->openFile( mDir->absolutePath() );
	}
	QFileInfoList fileInfoList = QDir::drives();
	if(fileInfoList.contains(mModel->fileName(index)))
	{
		mComboBox->setCurrentIndex(mComboBox->findText(mModel->fileName(index).remove(-1,1)));
	}
}

void QFileBrowserChild::setDrivers(const QString& string)
{
	mListView->setRootIndex(mModel->index(string));
}

void QFileBrowserChild::showEvent ( QShowEvent * event )
{
	mListView->resize(mDock->width(), mDock->height());	
}

QDockWidget* QFileBrowserChild::dock()
{ 
	return mDock; 
}
