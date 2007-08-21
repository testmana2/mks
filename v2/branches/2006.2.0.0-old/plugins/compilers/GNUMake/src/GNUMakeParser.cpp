#include "GNUMakeParser.h"
//
#include <QRegExp>
#include <QStringList>
//
QRegExp rxErrWarn;
QRegExp rxBuild;
QRegExp rxUndefRef;
QRegExp rxNoRule;
//
GNUMakeParser::GNUMakeParser( const QString& s1, const QString& s2, const QString& s3, const QString& s4 )
{
	rxErrWarn.setPattern( s1 );
	rxBuild.setPattern( s2 );
	rxUndefRef.setPattern( s3 );
	rxNoRule.setPattern( s4 );
}
//
QString GNUMakeParser::resultString()
{
	int e = 0, w = 0, c = 0, u = 0;
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
		case Compiling:
			c++;
			break;
		default:
			u++;
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
	// errors
	if ( e )
	{
		// message
		m.mText = tr( "Build failed" );
		m.mType = Bad;
		// result string
		s.append( tr( "Build failed." ) );
		//
		s.append( tr( " %1 error(s)," ).arg( colorText( QString::number( e ), "red" ) ) );
	}
	// no errors
	else
	{
		// message
		m.mText = tr( "Build complete" );
		m.mType = Good;
		// result string
		s.append( tr( "Build complete." ) );
	}
	// warnings
	if ( w )
		s.append( tr( " %1 warning(s)," ).arg( colorText( QString::number( w ), "red" ) ) );
	// compiling
	if ( c )
		s.append( tr( " %1 file(s) built," ).arg( colorText( QString::number( c ), "red" ) ) );
	// others
	if ( u )
		s.append( tr( " %1 unknow(s)," ).arg( colorText( QString::number( u ), "red" ) ) );
	// trimmin last , and repalce it by .
	if ( s.endsWith( "," ) )
		s[s.length() -1] = '.';
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
	if ( rxErrWarn.pattern().isEmpty() && rxBuild.pattern().isEmpty() && rxUndefRef.pattern().isEmpty() && rxNoRule.pattern().isEmpty() )
		return;
	//
	QStringList l = mBuffer.split( '\n' );
	foreach ( QString s, l )  
	{
		QString t;
		// compiling file
		if ( rxBuild.indexIn( s )  != -1 )
		{
			Message m;
			m.mFullText = rxBuild.cap( 0 );
			m.mFileName = QString::null;
			m.mPosition = QPoint( 0, 0 );
			m.mType = Compiling;
			m.mText =  tr( "Compiling %1..." ).arg( rxBuild.cap( 1 ) );
			mMessages.append( m );
			// emit signal
			emit newErrorAvailable( m );
			continue;
		}
		// error, warning in the source
		if (   rxErrWarn.indexIn( s ) != -1 )
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
			continue;
		}
		// undefined reference
 		if (   rxUndefRef.indexIn( s ) != -1 )
		{
			Message m;
			m.mFullText = rxUndefRef.cap( 0 );
			m.mFileName = QString::null;
			m.mPosition = QPoint( 0, 0 );
			m.mType = Error;
			m.mText =   rxUndefRef.cap( 1 )   //  in function 'main':main.cpp:
				+ rxUndefRef.cap( 2 ) ;  //undefined reference to ...
			mMessages.append( m );
			// emit signal
			emit newErrorAvailable( m );
			continue;
		}
		// no rule to make target ...
 		if (   rxNoRule.indexIn( s ) != -1 )
		{
			Message m;
			m.mFullText = rxNoRule.cap( 0 );
			m.mFileName = QString::null;
			m.mPosition = QPoint( 0, 0 );
			m.mType = Error;
			m.mText =   rxNoRule.cap( 1 ) ;
			mMessages.append( m );
			// emit signal
			emit newErrorAvailable( m );
			continue;
		}
	}
	mBuffer.clear();
}
