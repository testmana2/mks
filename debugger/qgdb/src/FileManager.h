#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

class QMdiArea;
class QsciScintilla;

class FileManager : public QObject
{
	Q_OBJECT
	
public:
	FileManager( QObject* parent, QMdiArea* mdiArea );
	virtual ~FileManager();

public slots:	
	void gotoFileLine( const QString& file, int line );

protected:
	QMdiArea* mMdiArea;
	
	QsciScintilla* openFile( const QString& fileName );

signals:
	void marginClicked( int margin, int line, Qt::KeyboardModifiers state );
};

#endif // FILEMANAGER_H
