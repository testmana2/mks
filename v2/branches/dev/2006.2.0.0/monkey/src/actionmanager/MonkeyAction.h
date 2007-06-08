#ifndef MONKEYACTION_H
#define MONKEYACTION_H
#include <QAction>

class MonkeyActGroup;

class MonkeyAction : public QAction
{
public:
	MonkeyAction ( MonkeyActGroup * parent, const QString & text, const QKeySequence & defaultShortcut);
	~MonkeyAction ();
private:
	void setShortcut ( const QKeySequence & shortcut );
};
#endif //MONKEYACTION_H