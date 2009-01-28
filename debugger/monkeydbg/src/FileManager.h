#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

#include "QGdbDriver.h"

class QMdiArea;
class pEditor;

class FileManager : public QObject
{
	Q_OBJECT
	
public:
	FileManager( QObject* parent, QMdiArea* mdiArea );
	virtual ~FileManager();

public slots:
	void openFileTriggered ();
	void closeFileTriggered ();
	
	void clearBreakpoints( const QString& file = QString::null, int line = -1 );
	void setBreakpoint( const QString& file, int line, int type );
	void clearDebuggerPosition();
	void setDebuggerPosition( const QString& file, int line );
	
	void breakpointAdded( const QGdbDriver::Breakpoint& breakpoint );
	void breakpointRemoved( const QGdbDriver::Breakpoint& breakpoint );
	
	void frameSelected( const QGdbDriver::Frame& frame );
	
	pEditor* openFile( const QString& fileName ); // was protected, moved for hack

protected slots:
	void marginClicked( int margin, int line, Qt::KeyboardModifiers state );

protected:
	QMdiArea* mMdiArea;

signals:
	void breakpointToggled( const QString& fileName, int line );
};

#endif // FILEMANAGER_H
