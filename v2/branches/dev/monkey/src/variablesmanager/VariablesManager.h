#ifndef VARIABLESMANAGER_H
#define VARIABLESMANAGER_H

#include <QHash>

#include "QSingleton.h"

typedef QHash <QString, QString> VarHash;

class VariablesManager: public QObject, public QSingleton<VariablesManager>
{
    Q_OBJECT
    friend class QSingleton<VariablesManager>;
private:
    VariablesManager ();

public:
    
    //Returns variable. 
    //Default value = QString::null used, if variable is not set
    QString getVariable (QString name,  VarHash locals); 

    //Is variable set
    bool isSet (QString name,  VarHash& locals); 

    //Will replace all variables in the string with values
    //If locals are presented, variables will be searched there too
    QString replaceAllVariables (QString str, VarHash locals);

private:
    //Hash for storing global constand variables. (Kernel version for example)
    VarHash globals;
};
#endif //VARIABLESMANAGER_H