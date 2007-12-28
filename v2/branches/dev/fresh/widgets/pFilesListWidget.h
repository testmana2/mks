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
#include <QListWidget>
#include <QDropEvent>

class pTabbedWorkspace;

class pFilesListWidget : public QDockWidget
{
	Q_OBJECT
public:
	pFilesListWidget(QString, QWidget* w, pTabbedWorkspace* p );
	~pFilesListWidget();
protected:
	QListWidget list;
    pTabbedWorkspace* mWorkspace;

	void dragEnterEvent (QDragEnterEvent*);
	void dropEvent (QDropEvent*);

protected slots:
	void showMenu ( const QPoint & );

	void setCurrentIndex (int);
    void modifiedChanged (int, bool);
    void documentInserted( int, QString, QIcon );
    void documentClosed ( int );

signals:
	void urlsDropped( const QList<QUrl>& );
};

#endif // PFILESLISTWIDGET_H
