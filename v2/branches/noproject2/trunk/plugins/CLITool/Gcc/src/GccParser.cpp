#include "GccParser.h"
#include <QDebug>

GccParser::GccParser()
{
	rxErrWarn.setPattern ("^(.+):(\\d+):(\\d+:)?\\s((warning)|(error)):\\s(.+)$");
	rxBuild.setPattern ("^g\\+\\+.+([^\\s]+\\.cpp)");
	rxUndefRef.setPattern ("^.*(In\\sfunction\\s.*:.*:).+(\\sundefined\\sreference\\sto.+)$");
	rxNoRule.setPattern ("^make: \\*\\*\\* (No rule to make target `.+', needed by `.+')\\.  Stop\\.$" );  // FIXME not a Gcc error

}

GccParser::~GccParser()
{
}


bool GccParser::parse(const QByteArray* arr)
{
	QStringList l = QString(*arr).split( '\n' );
	foreach (QString s, l)
	{
		// compiling file
		if ( rxBuild.indexIn( s )  != -1 )
		{
			pConsoleManager::Message m;
			m.mFullText = rxBuild.cap( 0 );
			m.mFileName = QString::null;
			m.mPosition = QPoint( 0, 0 );
			m.mType = pConsoleManager::Compiling;
			m.mText =  tr( "Compiling %1..." ).arg( rxBuild.cap( 1 ) );
			// emit signal
			emit newMessageAvailible( m );
			return true;
		}
		// error, warning in the source
		if (   rxErrWarn.indexIn( s ) != -1 )
		{
			pConsoleManager::Message m;
			m.mFullText = rxErrWarn.cap( 0 );
			m.mFileName = rxErrWarn.cap( 1 );
			m.mPosition = QPoint( 0, rxErrWarn.cap( 2 ).toInt() );
			if ( rxErrWarn.cap( 4 ).toLower() == "error" )
				m.mType = pConsoleManager::Error;
			else if ( rxErrWarn.cap( 4 ).toLower() == "warning" )
				m.mType = pConsoleManager::Warning;
			m.mText = QString( "%1:%2: %3" ).arg( m.mFileName ).arg( m.mPosition.y() ).arg( rxErrWarn.cap( 7 ) );
			// emit signal
			emit newMessageAvailible( m );
			return true;
		}
		// undefined reference
		if (   rxUndefRef.indexIn( s ) != -1 )
		{
			pConsoleManager::Message m;
			m.mFullText = rxUndefRef.cap( 0 );
			m.mFileName = QString::null;
			m.mPosition = QPoint( 0, 0 );
			m.mType = pConsoleManager::Error;
			m.mText =   rxUndefRef.cap( 1 )   //  in function 'main':main.cpp:
				+ rxUndefRef.cap( 2 ) ;  //undefined reference to ...
			// emit signal
			emit newMessageAvailible( m );
			return true;
		}
		// no rule to make target ...
		if (   rxNoRule.indexIn( s ) != -1 )
		{
			pConsoleManager::Message m;
			m.mFullText = rxNoRule.cap( 0 );
			m.mFileName = QString::null;
			m.mPosition = QPoint( 0, 0 );
			m.mType = pConsoleManager::Error;
			m.mText =   rxNoRule.cap( 1 ) ;
			// emit signal
			emit newMessageAvailible( m );
			return true;
		}
	}
return false;
}

QString GccParser::name() const
{
	return QString ("Gcc");
}
