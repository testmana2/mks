#include "GNUMakeParser.h"
//
#include <QRegExp>
#include <QStringList>
//
QRegExp rx;
//
GNUMakeParser::GNUMakeParser( const QString& s )
{
	setRegExp( s );
}
//
QString GNUMakeParser::resultString()
{
	int e = 0, w = 0, o = 0;
	foreach ( ConsoleCommandParser::ErrorInfos ce, mErrors )
	{
		switch ( ce.mType )
		{
		case ConsoleCommandParser::Error:
			e++;
			break;
		case ConsoleCommandParser::Warning:
			w++;
			break;
		default:
			o++;
			break;
		}
	}
	//
	QString s;
	QString b = colorText( "<br />[beta:If you see wrong message, please, send console output to the trac.monkeystudio.org]", "red" );
	if ( e )
		s = tr( "Make failed. %1 error(s), %2 warning(s) and %3 unknow(s)." ).append( b )
			.arg( colorText( QString::number( e ), "red" ), colorText( QString::number( w ), "red" ), colorText( QString::number( o ), "red" ) );
	else
		s = tr( "Make complete with %1 error(s), %2 warning(s) and %3 unknow(s)." ).append( b )
			.arg( colorText( QString::number( e ), "red" ), colorText( QString::number( w ), "red" ), colorText( QString::number( o ), "red" ) );
	return QString( "<b>%1</b>" ).arg( colorText( s, "green" ) );

}
//
void GNUMakeParser::appendToBuffer( const QString& s )
{
	mBuffer.append( s );
	if ( !mBuffer.endsWith( '\n' ) )
		return;
	// parse buffer
	parse();	
}
//
void GNUMakeParser::setRegExp( const QString& s )
{
	rx.setPattern( s );
}
//
void GNUMakeParser::parse()
{
	if ( rx.pattern().isNull() )
		return;
	//
	QStringList l = mBuffer.split( '\n' );
	foreach ( QString s, l )
	{
		int i = 0;
		QString t;
		while ( ( i = rx.indexIn( s, i ) ) != -1 )
		{
			ErrorInfos e;
			e.mFullText = rx.cap( 0 );
			e.mFileName = rx.cap( 1 );
			e.mPosition = QPoint( 0, rx.cap( 2 ).toInt() );
			t = rx.cap( 4 ).toLower();
			if ( t == "error" )
				e.mType = ConsoleCommandParser::Error;
			else if ( t == "warning" )
				e.mType = ConsoleCommandParser::Warning;
			e.mText = rx.cap( 7 );
			// add error to list
			mErrors.append( e );
			// emit signal
			emit newErrorAvailable( e );
			// update i
			i += rx.matchedLength();
		}
	}
	mBuffer.clear();
}
