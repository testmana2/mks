#include "Gcc.h"
#include "GccParser.h"

QString Gcc:name()
{
    return QString ("Gcc");
}

QString* Gcc::getCommand ()
{
    //here possible will be some code, determining needed version of compiler ( gcc-4.1, etc )
    return QString ("gcc"); 
}

QString getSwitches (QString oldSwitches = QString::null)
{
    //here possible will be code, showing to user config dialog
    return oldSwitches
}

pCommandParser* Gcc::getParser()
{
    return new GccParser;
}