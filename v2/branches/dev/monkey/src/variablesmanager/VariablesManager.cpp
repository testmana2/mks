#include "VariablesManager.h"

#include <QStringList>
#include <QList>
#include <QRegExp>

QString VariablesManager::getVariable (QString name, VarHash& locals)
{
	// TODO realise support of global variables, that must be recieved some way
	// $cpp$ for example
	if ( globals.contains(name))
		return globals[name];
	if (locals.contains(name))
		return locals[name];
	return QString::null;
}

bool VariablesManager::isSet (QString name, VarHash& locals)
{
	// TODO realise support of global variables, that must be recieved some way
	// $cpp$ for example
	return (globals.contains(name) || locals.contains(name));
}

QString VariablesManager::replaceAllVariables (QString text, VarHash& locals)
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