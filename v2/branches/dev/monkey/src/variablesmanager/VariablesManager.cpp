#include "VariablesManager.h"

#include <QStringList>
#include <QList>
#include <QRegExp>
#include <QDateTime>

VariablesManager::VariablesManager ()
{};

QString VariablesManager::getVariable (QString name, VarHash locals)
{
    QString result = QString::null;
    // monkeystudio_version
    if ( name == "$editor_version$" )
        result = PROGRAM_VERSION;
    // monkeystudio_version_string
    else if ( name == "$editor_version_string$" )
        result = QString( "%1 v%2" ).arg( PROGRAM_NAME ).arg( PROGRAM_VERSION );
    else if ( name == "$date$" )
        result = QDateTime::currentDateTime().toString( Qt::ISODate );
    if (!result.isEmpty())
        return result;
    if ( globals.contains(name))
        return globals[name];
    if (locals.contains(name))
        return locals[name];
    return QString::null;
}

bool VariablesManager::isSet (QString name, VarHash& locals)
{
    if (    name == "editor_version" ||
            name == "editor_version_string" ||
            name == "date" )
        return true;
    return (globals.contains(name) || locals.contains(name));
}

QString VariablesManager::replaceAllVariables (QString text, VarHash locals)
{
    int p = 0;
    QString s;
    QRegExp rex( "(\\$[^$\\s]+\\$)" );
    // search and interpret values
    QList<QString> findedVariables;
    while ( ( p = rex.indexIn( text, p ) ) != -1 )
    {
        // got keyword
        s = rex.capturedTexts().value( 1 );
        findedVariables.append (s);
        p += rex.matchedLength();
    }
    // replace occurences
    foreach ( QString s, findedVariables )
        text.replace( s, getVariable(s,locals), Qt::CaseInsensitive );
    // return value
    return text;
}