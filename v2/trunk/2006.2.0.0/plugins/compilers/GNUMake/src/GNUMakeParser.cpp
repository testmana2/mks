#include "GNUMakeParser.h"
//
#include <QRegExp>
#include <QStringList>
//
QRegExp rxErrWarn;
QRegExp rxBuild;
//
GNUMakeParser::GNUMakeParser( const QString& s1, const QString& s2 )
{
	rxErrWarn.setPattern( s1 );
	rxBuild.setPattern( s2 );
}
//
QString GNUMakeParser::resultString()
{
	int e = 0, w = 0, o = 0;
	foreach ( Message m, mMessages )
	{
		switch ( m.mType )
		{
		case Error:
			e++;
			break;
		case Warning:
			w++;
			break;
		default:
			o++;
			break;
		}
	}
	// message
	Message m;
	m.mFileName = QString::null;
	m.mPosition = QPoint( 0, 0 );
	// result string
	QString s;
	QString b = colorText( "<br />[beta:If you see wrong message, please, send console output to the trac.monkeystudio.org]", "red" );
	if ( e )
	{
		// message
		m.mText = tr( "Build failed" );
		m.mType = Bad;
		// result string
		s = tr( "Make failed. %1 error(s), %2 warning(s) and %3 unknow(s)." ).append( b )
			.arg( colorText( QString::number( e ), "red" ), colorText( QString::number( w ), "red" ), colorText( QString::number( o ), "red" ) );
	}
	else
	{
		// message
		m.mText = tr( "Build complete" );
		m.mType = Good;
		// result string
		s = tr( "Make complete with %1 error(s), %2 warning(s) and %3 unknow(s)." ).append( b )
			.arg( colorText( QString::number( e ), "red" ), colorText( QString::number( w ), "red" ), colorText( QString::number( o ), "red" ) );
	}
	// message
	m.mFullText = s;
	mMessages.append( m );
	emit newErrorAvailable( m );
	// result string
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
void GNUMakeParser::parse()
{
	if ( rxErrWarn.pattern().isNull() )
		return;
	//
	QStringList l = mBuffer.split( '\n' );
	foreach ( QString s, l )
	{
		int i = 0;
		QString t;
		while ( ( i = rxErrWarn.indexIn( s, i ) ) != -1 )
		{
			Message m;
			m.mFullText = rxErrWarn.cap( 0 );
			m.mFileName = rxErrWarn.cap( 1 );
			m.mPosition = QPoint( 0, rxErrWarn.cap( 2 ).toInt() );
			t = rxErrWarn.cap( 4 ).toLower();
			if ( t == "error" )
				m.mType = Error;
			else if ( t == "warning" )
				m.mType = Warning;
			m.mText = QString( "%1:%2: %3" ).arg( m.mFileName ).arg( m.mPosition.y() ).arg( rxErrWarn.cap( 7 ) );
			// add error to list
			mMessages.append( m );
			// emit signal
			emit newErrorAvailable( m );
			// update i
			i += rxErrWarn.matchedLength();
		}
		//
		i = 0;
		while ( ( i = rxBuild.indexIn( s, i ) ) != -1 )
		{
			Message m;
			m.mFullText = rxBuild.cap( 0 );
			m.mFileName = QString::null;
			m.mPosition = QPoint( 0, 0 );
			m.mType = State;
			m.mText =  tr( "Compiling %1..." ).arg( rxBuild.cap( 1 ) );
			mMessages.append( m );
			// emit signal
			emit newErrorAvailable( m );
			// update i
			i += rxBuild.matchedLength();
		}
	}
	mBuffer.clear();
}
