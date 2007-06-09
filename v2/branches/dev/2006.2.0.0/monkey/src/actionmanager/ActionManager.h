#include <QObject>
#include <QAction>
#include <QActionGroup>
#include <QString>
#include <QDialog>
#include <QKeySequence>

#include "MonkeyAction.h"
#include "MonkeyActGroup.h"

class ActionManager : public QObject
{
Q_OBJECT
private:

	QList <MonkeyActGroup*> actionGroups;

	static ActionManager* mSelf;	

public:
	static ActionManager* self ();
    
    ActionManager ();
	void addActGroup ( MonkeyActGroup* grp);
	void deleteActGroup ( MonkeyActGroup* grp);
    
	static QKeySequence getShortCut ( MonkeyAction* );
	QDialog* shotcutsConfig ();
    
public slots:
    void showSettings ();
};
