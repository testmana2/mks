#ifndef MENUTOOLS_H
#define MENUTOOLS_H
//
#include <QObject>
#include <QPointer>
//
class QAction;
//
class MenuTools : public QObject
{
	Q_OBJECT
	//
public:
	static MenuTools* self( QObject* = 0 );
	//
	void initialize();
	//
private:
	MenuTools( QObject* = 0 );
	//
	static QPointer<MenuTools> mSelf;
	//
public slots:
	void triggered( QAction* );
};
//
#endif // MENUTOOLS_H
