#ifndef GCC_H
#define GCC_H

#include "BasePlugin.h"

class Gcc : public CLIToolPlugin
{
    Q_OBJECT
    Q_INTERFACES( CLIToolPlugin )

public:
    Gcc();
    virtual ~Gcc();
    bool setEnabled( bool );

    QString name();
    
    //Command for executing tool
    pCommand* getCommand ();
    
    //will show to user dialog, where he can configure command line switches, 
    //and, when user will accept, switches string will be returned
    QString getSwitches (QString oldSwitches = QString::null);

    //returns pointer to the console output parser for output of console utility
    pCommandParser* getParser();

};

#endif
