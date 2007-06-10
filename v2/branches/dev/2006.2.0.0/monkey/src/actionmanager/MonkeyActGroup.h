#ifndef MONKEYACTGROUP_H
#define MONKEYACTGROUP_H
#include <QActionGroup>
#include <QWidget>

class MonkeyAction;

class ActionManager;

class MonkeyActGroup : public QActionGroup
{
public:
//	QList <MonkeyAction*> actions;

	MonkeyActGroup ( const QString & name, QWidget * parentWidget );
	~MonkeyActGroup ();

//	void appendAction ( MonkeyAction*);
//	void removeAction ( MonkeyAction* );
};
#endif //MONKEYACTGROUP_H