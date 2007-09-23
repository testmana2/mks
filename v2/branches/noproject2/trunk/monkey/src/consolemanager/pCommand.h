/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : P@sNox
** Project   : pCommand
** FileName  : pCommand.h
** Date      : jeu. 23. août 15:20:31 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PCOMMAND_H
#define PCOMMAND_H

#include "MonkeyExport.h"
#include "pCommandParser.h"

#include <QObject>
#include <QStringList>
#include <QPointer>

class Q_MONKEY_EXPORT pCommand : public QObject
{
	Q_OBJECT

public:
	pCommand( QObject* o = 0 )	: QObject( o ) {}
	~pCommand() {}

	QString text() const { return mText; }
	QString command() const;
	QStringList arguments() const { return mArguments; }
	QString workingDirectory() const { return mWorkingDirectory; }
	QStringList parsers() const { return mParsers; }
	bool tryAllParsers() const {return mTryAllParsers; }
	bool skipOnError() const { return mSkipOnError; }

	void setText( const QString& s ) { mText = s; }
	void setDefaultCommand( const QString& s ) { mDefaultCommand = s; }
	void setArguments( const QStringList& l ) { mArguments = l; }
	void setWorkingDirectory( const QString& s ) { mWorkingDirectory = s; }
	void setParsers( QStringList parsers ) { mParsers = parsers; }
	void setTryAllParsers (bool b) { mTryAllParsers = true; }
	void setSkipOnError( bool b ) { mSkipOnError = b; }

protected:
	QString mText;
	QString mTool; //Text name of a tool
	QString mDefaultCommand; //command of a tool, that will be used, if mTool will not be finded
	QStringList mArguments;//for command without mTool may also be stored in the mDefauldCommand
	QString mWorkingDirectory;
	bool mSkipOnError;
	QStringList mParsers;
	bool mTryAllParsers; //if output not parsed by mParsers, will try to parse by all other parsers
};

typedef QList<pCommand*> pCommandList;

#endif // PCOMMAND_H
