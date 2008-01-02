/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/12/29
 * AUTHOR       : Kopats Andrei aka hlamer( hlamer@tut.by )
 * FILENAME     : pFilesListWidget.h
 * LICENSE      : GPL
 * COMMENTARY   : Widget with list of files, opened on workspace, for activating needed file
 ********************************************************************************************************/
#ifndef PFILESLISTWIDGET_H
#define PFILESLISTWIDGET_H

#include <QDockWidget>
#include <QUrl>

class pTabbedWorkspace;
class QListWidget;
class QDragEnterEvent;
class QDropEvent;

class pFilesListWidget : public QDockWidget
{
	Q_OBJECT
	
public:
	pFilesListWidget( const QString&, pTabbedWorkspace* );

protected:
	QListWidget* mList;
    pTabbedWorkspace* mWorkspace;

	void dragEnterEvent( QDragEnterEvent* );
	void dropEvent( QDropEvent* );

protected slots:
    void modifiedChanged( int, bool );
    void docTitleChanged( int, const QString& );
    void documentInserted( int, const QString&, const QIcon& );
    void documentClosed( int );
	void setCurrentRow( int );

signals:
	void urlsDropped( const QList<QUrl>& );
};

#endif // PFILESLISTWIDGET_H
