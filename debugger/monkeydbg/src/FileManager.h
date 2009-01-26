#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

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
	
	void clearBreakpoints();
	void clearDebuggerPosition();
	void setDebuggerPosition( const QString& file, int line );
	
	pEditor* openFile( const QString& fileName ); // was protected, moved for hack

protected slots:
	void marginClicked( int margin, int line, Qt::KeyboardModifiers state );

protected:
	QMdiArea* mMdiArea;

signals:
	void breakpointToggled( const QString& fileName, int line, bool& remove );
};

#endif // FILEMANAGER_H
