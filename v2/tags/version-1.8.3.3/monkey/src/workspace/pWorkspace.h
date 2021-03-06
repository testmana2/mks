/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pWorkspace.h
** Date      : 2008-01-14T00:37:22
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#ifndef PWORKSPACE_H
#define PWORKSPACE_H

#include <fresh.h>

#include "../consolemanager/pConsoleManager.h"

class pAbstractChild;
class QMainWindow;
class XUPProjectItem;
class QFileSystemWatcher;
class QTimer;

class Q_MONKEY_EXPORT pWorkspace : public pExtendedWorkspace
{
	Q_OBJECT
	friend class MonkeyCore;

public:
	void loadSettings();
	pAbstractChild* currentChild() const;
	pAbstractChild* child( int ) const;
	QList<pAbstractChild*> children() const;
	
	void addSearhReplaceWidget (QWidget* widget);
	static QString defaultContext();
	void initMultiToolBar( QToolBar* tb );
	
protected:
	QFileSystemWatcher* mFileWatcher;
	QTimer* mContentChangedTimer;
	static int CONTENT_CHANGED_TIME_OUT;
	static QString DEFAULT_CONTEXT;
	
	virtual void closeDocument( QWidget* document );

private:
	pWorkspace( QMainWindow* = 0 );
	
public slots:
	// ask the user a filename and create / open the file
	pAbstractChild* newTextEditor();
	// open a file and set cursor to position
	void initChildConnections( pAbstractChild* child );
	pAbstractChild* openFile( const QString& fileName, const QString& codec );
	// close the file
	void closeFile( const QString& );
	// goto to position inside file, highlight line according to bool, opening it if needed
	void goToLine( const QString& fileName, const QPoint& pos, bool highlight, const QString& codec );

	void closeCurrentDocument();
	bool closeAllDocuments();
	
	void fileWatcher_alertClicked( QDialogButtonBox::StandardButton button, const pQueuedMessage& message );

protected slots:
	void internal_contentChanged();
	void internal_currentFileChanged( const QString& fileName );
	void internal_currentChanged( int id );
	//void internal_aboutToCloseTab( int, QCloseEvent* );
	void internal_urlsDropped( const QList<QUrl>& );
	void internal_listWidget_customContextMenuRequested( const QPoint& );
	void internal_projectsManager_customContextMenuRequested( const QPoint& pos );
	void internal_currentProjectChanged( XUPProjectItem* currentProject, XUPProjectItem* previousProject );
	void internal_projectInstallCommandRequested( const pCommand& cmd, const QString& mnu );
	void internal_projectUninstallCommandRequested( const pCommand& cmd, const QString& mnu );
	void projectCustomActionTriggered();
	void contentChangedTimer_timeout();
	void fileWatcher_ecmNothing( const QString& filename );
	void fileWatcher_ecmReload( const QString& filename, bool force = false );
	void fileWatcher_ecmAlert( const QString& filename );
	void fileWatcher_fileChanged( const QString& path );

public slots:
	// file menu
	void fileNew_triggered();
	void fileOpen_triggered();
	void fileSessionSave_triggered();
	void fileSessionRestore_triggered();
	void fileSaveCurrent_triggered();
	void fileSaveAll_triggered();
	void fileCloseCurrent_triggered();
	void fileCloseAll_triggered();
	void fileSaveAsBackup_triggered();
	void fileQuickPrint_triggered();
	void filePrint_triggered();
	void fileExit_triggered();

	// edit menu
	void editSettings_triggered();
	void editTranslations_triggered();
	void editUndo_triggered();
	void editRedo_triggered();
	void editCut_triggered();
	void editCopy_triggered();
	void editPaste_triggered();
	void editSearch_triggered();
	void editGoTo_triggered();
	void editExpandAbbreviation_triggered();
	void editPrepareAPIs_triggered();

	// help menu
	void helpAboutApplication_triggered();
	void helpAboutQt_triggered();
#ifdef __COVERAGESCANNER__
	void helpTestReport_triggered();
#endif
	// no menu
	void focusToEditor_triggered ();

signals:
	// a file has been opened
	void fileOpened( const QString& fileName );
	// a file has been closed
	void fileClosed( const QString& fileName );
	// a file have changed
	void fileChanged( const QString& fileName );
	// current file changed
	void currentFileChanged( pAbstractChild*, const QString& );
	// content changed
	void contentChanged();
};

#endif // PWORKSPACE_H
