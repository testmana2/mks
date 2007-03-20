#ifndef QFILEBROWSERCHILD_H
#define QFILEBROWSERCHILD_H

#include <QPointer>
class Workspace;
class QDockWidget;
class QVBoxLayout;
class QListView;
class QWidget;
class QLabel;

class QFileBrowserChild : public QObject
{
	Q_OBJECT
public:
	static QFileBrowserChild* self( Workspace* = 0 );
	QDockWidget* dock();
private:
	QFileBrowserChild( Workspace* );
	Workspace* mWorkspace;
	QDockWidget* mDock;
	static QPointer<QFileBrowserChild> mSelf;
	QVBoxLayout* mLayout;
	QWidget* mWidget;
	QListView* mListView;
	QLabel* mLabel;
};

#endif
