#ifndef PROJECT_H
#define PROJECT_H

#include "MonkeyExport.h"
#include "ProjectItem.h"

class Q_MONKEY_EXPORT Project : public QObject, public ProjectItem
{
Q_OBJECT
public:
enum BuildActionType
{
    aBuild = 0,
    aClean ,
    aReBuild,
    aDistClean,
    aExecute,
    aBuidExecute,
    aCustom1,
    aCustom2,
    aCustom3,
    aCustom4,
    aCustom5,
    aCustom6,
    aLast
}
struct BuildAction
{
    QString text();
    QString command ();
}

    Project ();
    ~Project ();

    virtual bool isEnabled();
    virtual ProjectPlugin* getParentPlugin ();

private:
    void processAction (BuildActionType);
    void getCommand (BuildActionType);
    void BuildAction actions[aLast] // [item count in the enum

public slots:
    virtual void editSettings() {};

    // the function that open a project
    virtual bool openProject( const QString&, ProjectItem* = 0 ) {return false;};

};
#endif // PROJECTITEM_H
