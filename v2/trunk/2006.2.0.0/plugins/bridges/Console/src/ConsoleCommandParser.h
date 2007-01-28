#ifndef CONSOLECOMMANDPARSER_H
#define CONSOLECOMMANDPARSER_H
//
#include <QObject>
#include <QPoint>
#include <QList>
//
class ConsoleCommandParser : public QObject
{
	Q_OBJECT
	//
public:
	enum MessageType { Unknow = -1, Error, Warning, Good, Bad, State };
	struct Message
	{
		QString mFileName;
		QPoint mPosition;
		ConsoleCommandParser::MessageType mType;
		QString mText;
		QString mFullText;
	};
	//
	virtual ~ConsoleCommandParser() { mMessages.clear(); }
	//
	virtual QString resultString() = 0;
	virtual QString colorText( const QString& s, const QString& c )
	{ return QString( "<font color=\"%1\">%2</font>" ).arg( c, s ); }
	//
	virtual void appendToBuffer( const QString& ) = 0;
	//
protected:
	QList<ConsoleCommandParser::Message> mMessages;
	QString mBuffer;
	//
signals:
	void newErrorAvailable( const ConsoleCommandParser::Message& );
	//
};
//
#endif // CONSOLECOMMANDPARSER_H
