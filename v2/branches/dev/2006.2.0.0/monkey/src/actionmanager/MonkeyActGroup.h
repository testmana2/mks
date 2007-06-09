#ifndef MONKEYACTGROUP_H
#define MONKEYACTGROUP_H
#include <QActionGroup>
#include <QWidget>

class MonkeyAction;

class ActionManager;

class MonkeyActGroup : public QActionGroup
{
private:
	QList <MonkeyAction*> actions;
public:
	MonkeyActGroup ( const QString & name, QWidget * parentWidget );
	~MonkeyActGroup ();

	void appendAction ( MonkeyAction*);
	void removeAction ( MonkeyAction* );
};
#endif //MONKEYACTGROUP_H