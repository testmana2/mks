#ifndef GCC_H
#define GCC_H

#include "CLIToolPlugin.h"

class Gcc : public CLIToolPlugin
{
    Q_OBJECT
    Q_INTERFACES( BasePlugin CLIToolPlugin )

public:
	Gcc ();

	bool setEnabled( bool );

    QString name();

    //Command for executing tool
	QString getCommand ();

    //will show to user dialog, where he can configure command line switches, 
    //and, when user will accept, switches string will be returned
    QString getSwitches (QString oldSwitches = QString::null);

    //returns pointer to the console output parser for output of console utility
    pCommandParser* generateParser();

};

#endif //GCC_H
