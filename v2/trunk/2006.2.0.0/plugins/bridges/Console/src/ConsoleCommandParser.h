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
	enum ErrorType { Unknow = -1, Error, Warning };
	struct ErrorInfos
	{
		QString mFileName;
		QPoint mPosition;
		ConsoleCommandParser::ErrorType mType;
		QString mText;
		QString mFullText;
	};
	//
	virtual ~ConsoleCommandParser() { mErrors.clear(); }
	//
	virtual QString resultString() = 0;
	virtual QString colorText( const QString& s, const QString& c )
	{ return QString( "<font color=\"%1\">%2</font>" ).arg( c, s ); }
	//
	virtual void appendToBuffer( const QString& ) = 0;
	virtual void setRegExp( const QString& ) = 0;
	//
protected:
	QList<ConsoleCommandParser::ErrorInfos> mErrors;
	QString mBuffer;
	//
signals:
	void newErrorAvailable( const ConsoleCommandParser::ErrorInfos& );
	//
};
//
#endif // CONSOLECOMMANDPARSER_H
