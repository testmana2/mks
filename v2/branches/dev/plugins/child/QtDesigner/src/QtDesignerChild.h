/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Child Plugins
** FileName  : QtDesignerChild.h
** Date      : 2008-01-14T00:57:16
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
#ifndef QTDESIGNERCHILD_H
#define QTDESIGNERCHILD_H

#include <fresh.h>
#include <pWorkspace.h>
#include <pAbstractChild.h>

namespace SharedTools {
	class WidgetHost;
};

class QToolBar;
class QtDesignerManager;
class QDesignerFormWindowInterface;

class QtDesignerChild : public pAbstractChild
{
	Q_OBJECT
	
public:
	QtDesignerChild( QtDesignerManager* manager );
	~QtDesignerChild();
	
protected:
	QtDesignerManager* mDesignerManager;
	SharedTools::WidgetHost* mHostWidget;
	
	void showEvent( QShowEvent* event );

protected slots:
	void formChanged();
	void formSelectionChanged();
	void formGeometryChanged();
	void formMainContainerChanged( QWidget* widget );
	
public:
	virtual QStringList files() const;
	virtual QString fileBuffer( const QString& fileName, bool& ok ) const;
	virtual QString context() const;
	virtual void initializeContext( QToolBar* tb );
	virtual QPoint cursorPosition() const;
	virtual void showFile( const QString& );
	virtual QString currentFile() const;
	virtual QString currentFileName() const;
	inline virtual pEditor* currentEditor() const { return 0; }
	virtual bool isModified() const;
	virtual bool isUndoAvailable() const;
	virtual bool isRedoAvailable() const;
	virtual bool isPasteAvailable() const;
	virtual bool isCopyAvailable() const;
	virtual bool isModified( const QString& ) const;
	virtual void saveFile( const QString& );
	virtual void saveFiles();
	virtual void printFile( const QString& );
	virtual void quickPrintFile( const QString& );
	virtual void undo();
	virtual void redo();
	virtual void cut();
	virtual void copy();
	virtual void paste();
	virtual void searchReplace();
	virtual void goTo();
	virtual bool isSearchReplaceAvailable() const;
	virtual bool isGoToAvailable() const;
	virtual bool isPrintAvailable() const;
	
public slots:
	virtual void searchPrevious() {}
	virtual void searchNext() {}
	virtual void goTo( const QString&, const QPoint&, bool = false );
	virtual void backupCurrentFile( const QString& );
	virtual bool openFile( const QString& fileName, const QString& codec );
	virtual void closeFile( const QString& );
	virtual void closeFiles();
	
};

#endif // QTDESIGNERCHILD_H
