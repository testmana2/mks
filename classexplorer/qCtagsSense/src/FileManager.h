#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QModelIndex>
#include <QMap>
#include <QPointer>

class QPlainTextEdit;
class QTimer;

class FileManager : public QObject
{
	Q_OBJECT
	
public:
	FileManager( QObject* parent = 0 );
	virtual ~FileManager();

public slots:
	QPlainTextEdit* openFile( const QString& fileName );
	void goTo( const QString& fileName, const QPoint& pos );
	void memberActivated( const QString& fileName, const QModelIndex& index );

protected:
	QMap<QString, QPointer<QPlainTextEdit> > mEditors;
	QTimer* mFileModifiedTimer;

protected slots:
	void bufferModified();
	void internal_bufferModified_timeout();

signals:
	void buffersModified( const QMap<QString, QString>& buffers ); // fileName, content
};

#endif // FILEMANAGER_H
