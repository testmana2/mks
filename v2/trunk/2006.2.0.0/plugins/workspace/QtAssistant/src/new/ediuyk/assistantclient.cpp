/****************************************************************************
**
** Copyright (C) 2006 FullMetalCoder
**
** This file is part of the Edyuk project (beta version)
** 
** This file may be used under the terms of the GNU General Public License
** version 2 as published by the Free Software Foundation and appearing in the
** file GPL.txt included in the packaging of this file.
**
** Notes :	Parts of the project are derivative work of Trolltech's QSA library
** or Trolltech's Qt4 framework but, unless notified, every single line of code
** is the work of the Edyuk team or a contributor. 
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "assistantclient.h"

#include "assistant.h"

#include <DevGUI>
#include <qmdiServer>
#include <EdyukApplication>

#include <QUrl>
#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QComboBox>
#include <QFileInfo>
#include <QStatusBar>
#include <QGridLayout>
#include <QTextBrowser>
#include <QContextMenuEvent>

class AssistantBrowser : public QTextBrowser
{
	public:
		AssistantBrowser(QWidget *p = 0) : QTextBrowser(p) {}
		
		virtual void setSource(const QUrl& u)
		{
			QUrl url(u);
			QString scheme = url.scheme();
			
			if ( scheme == "ftp" || scheme == "http" || scheme == "https" ||
				 scheme == "svn" || scheme == "sftp" || scheme == "rsync" )
			{
				qWarning("Web-connection unsupported...");
				return;
			}
			
			if ( scheme.isEmpty() )
				url.setScheme("file");
			
			QTextBrowser::setSource(url);
		}
};

AssistantClient::AssistantClient(Assistant *a, QWidget *p)
 : qmdiWidget(p), pOwner(a)
{
	pUrl = new QComboBox(this);
	pUrl->setEditable(true);
	pUrl->setAutoCompletion(true);
	pUrl->setDuplicatesEnabled(false);
	
	pBrowser = new AssistantBrowser(this);
	pBrowser->setContextMenuPolicy(Qt::NoContextMenu);
	pBrowser->setSearchPaths( QStringList( Assistant::docPath() ) );
	
	pLabel = new QLabel(tr("Address : "), this);
	
	QGridLayout *grid = new QGridLayout(this);
	grid->setMargin(5);
	grid->setSpacing(0);
	
	grid->addWidget(pLabel	, 0, 1, 01, 05);
	grid->addWidget(pUrl	, 0, 6, 01, 54);
	
	
	grid->addWidget(pBrowser, 1, 0, 47, 64);
	
	setLayout(grid);
	
	pMenu = new QMenu;
	
	aForward = new QAction(QIcon(":/forward.png"), tr("Forward"), this);
	aForward->setEnabled(false);
	connect(pBrowser, SIGNAL( forwardAvailable(bool) ),
			aForward, SLOT  ( setEnabled(bool) ) );
	connect(aForward, SIGNAL( triggered() ),
			pBrowser, SLOT  ( forward() ) );
	
	aBackward = new QAction(QIcon(":/backward.png"), tr("Backward"), this);
	aForward->setEnabled(false);
	connect(pBrowser , SIGNAL( backwardAvailable(bool) ),
			aBackward, SLOT  ( setEnabled(bool) ) );
	connect(aBackward, SIGNAL( triggered() ),
			pBrowser , SLOT  ( backward() ) );
	
	aReload = new QAction(QIcon(":/reload.png"), tr("Reload"), this);
	connect(aReload	, SIGNAL( triggered() ),
			pBrowser, SLOT  ( reload() ) );
	
	aHome = new QAction(QIcon(":/home.png"), tr("Home"), this);
	connect(aHome	, SIGNAL( triggered() ),
			pBrowser, SLOT  ( home() ) );
	
	aOpenLink = new QAction(tr("Open link"), this);
	connect(aOpenLink	, SIGNAL( triggered() ),
			this		, SLOT  ( openLink() ) );
	
	aOpenInTab = new QAction(tr("Open in new tab"), this);
	connect(aOpenInTab	, SIGNAL( triggered() ),
			this		, SLOT  ( openInNewTab() ) );
	
	toolbars["Edit"]->addAction(aBackward);
	toolbars["Edit"]->addAction(aForward);
	toolbars["Edit"]->addSeparator();
	toolbars["Edit"]->addAction(aReload);
	toolbars["Edit"]->addAction(aHome);
	
	retranslate();
	
	connect(pBrowser, SIGNAL( sourceChanged(const QUrl&) ),
			this	, SLOT  ( sourceChanged(const QUrl&) ) );
	
	connect(pUrl, SIGNAL( currentIndexChanged(const QString&) ),
			this, SLOT  ( currentIndexChanged(const QString&) ) );
	
	connect(pBrowser, SIGNAL( highlighted(const QString&) ),
			this	, SLOT  ( highlighted(const QString&) ) );
	
	setContentModified(false);
}

AssistantClient::~AssistantClient()
{
	assistant()->removeClient(this);
}

void AssistantClient::openLink(const QString& lnk)
{
	pBrowser->setSource(lnk);
}

void AssistantClient::openInNewTab(const QString& lnk)
{
	AssistantClient *a = assistant()->createClient();
	a->openLink(lnk);
	
	if ( server() )
		server()->addClient(a);
}

void AssistantClient::retranslate()
{
	pLabel->setText(tr("Address : "));
	
	aForward->setText(tr("Forward"));
	aBackward->setText(tr("Backward"));
	aReload->setText(tr("Reload"));
	aHome->setText(tr("Home"));
	
	aOpenLink->setText(tr("Open link"));
	aOpenInTab->setText(tr("Open in new tab"));
	
	toolbars.setTranslation("Edit", tr("Edit"));
	
}

void AssistantClient::contextMenuEvent(QContextMenuEvent *e)
{
	e->accept();
	
	anchor = pBrowser->anchorAt( pBrowser->mapFromParent( e->pos() ) );
	
	pMenu->clear();
	
	if ( !anchor.isEmpty() )
	{
		if ( anchor.at(0) == '#' )
		{
			QString src = pBrowser->source().toString();
			int hash = src.indexOf('#');
			
			anchor.prepend( (hash > -1) ? src.left(hash) : src );
		}
		
		pMenu->addAction(aOpenLink);
		pMenu->addAction(aOpenInTab);
		pMenu->addSeparator();
	}
	
	pMenu->addAction(aBackward);
	pMenu->addAction(aForward);
	pMenu->addSeparator();
	pMenu->addAction(aReload);
	pMenu->addAction(aHome);
	pMenu->addSeparator();
	pMenu->addActions( pBrowser->createStandardContextMenu()->actions() );
	
	pMenu->exec(e->globalPos());
}

void AssistantClient::openLink()
{
	if ( anchor.count() )
		openLink(anchor);
}

void AssistantClient::openInNewTab()
{
	if ( anchor.count() )
		openInNewTab(anchor);
}

void AssistantClient::highlighted(const QString& link)
{
	COMPONENT(gui)->statusBar()->showMessage(link);
}

void AssistantClient::sourceChanged(const QUrl& url)
{
	pUrl->setEditText(url.toString());
	
	setTitle(	tr("Assistant : %1").arg(
				QFileInfo(url.toString()).completeBaseName() )
			);
}

void AssistantClient::currentIndexChanged(const QString& s)
{
	openLink(s);
}
