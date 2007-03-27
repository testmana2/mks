#ifndef QFILEBROWSERCHILD_H
#define QFILEBROWSERCHILD_H

#include <QPointer>
class Workspace;
class QDockWidget;
class QVBoxLayout;
class QListView;
class QWidget;
class QLabel;
class QDirModel;
class QMouseEvent;
class QModelIndex;
class QComboBox;
class QDir;
class QShowEvent;

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
	QDir* mDir;
	QDirModel* mModel;
	QComboBox* mComboBox;
private slots:
	void clickReaction(const QModelIndex&);
	void setDrivers(const QString&);
protected:
	void showEvent(QShowEvent*);
};

#endif
