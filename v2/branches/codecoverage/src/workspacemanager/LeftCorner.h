#ifndef LEFTCORNER_H
#define LEFTCORNER_H
//
#include <QToolButton>
#include <QPointer>
//
class QAction;
class QMenu;
class QActionGroup;
class Workspace;
//
class LeftCorner : public QToolButton
{
	Q_OBJECT
	//
public:
	static LeftCorner* self( Workspace* = 0 );
	//
private:
	LeftCorner( Workspace* = 0 );
	static QPointer<LeftCorner> mSelf;
	Workspace* mWorkspace;
	QMenu* mLayouts;
	QActionGroup* agFiles, *agLayouts;	
	//
private slots:
	void menu_aboutToShow();
	void agFiles_triggered( QAction* );
	void agLayouts_triggered( QAction* );
	//
};
//
#endif // LEFTCORNER_H
