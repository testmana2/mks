#include "QFileBrowserChild.h"
#include "Workspace.h"

#include <QDockWidget>
#include <QPointer>
#include <QListView>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>

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
	mDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	mLabel = new QLabel("blubb");
	mListView->resize(mDock->width(), mDock->height());
	mDock->setWidget(mListView);
}

QDockWidget* QFileBrowserChild::dock()
{ 
	return mDock; 
}
