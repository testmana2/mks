#ifndef MONKEYACTION_H
#define MONKEYACTION_H
#include <QAction>

class MonkeyActGroup;

class MonkeyAction : public QAction
{
public:
	MonkeyAction ( MonkeyActGroup * parent, const QString & text, const QString name, const QKeySequence & defaultShortcut);
	~MonkeyAction ();
};
#endif //MONKEYACTION_H