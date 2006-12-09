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

#ifndef _DEV_ASSISTANT_H_
#define _DEV_ASSISTANT_H_

#include "main.h"

#include <QString>
#include <qmdiWidget>

class Assistant;

class QUrl;
class QMenu;
class QLabel;
class QAction;
class QComboBox;
class QTextBrowser;

class AssistantClient : public qmdiWidget
{
	Q_OBJECT
	
	public:
		AssistantClient(Assistant *p, QWidget *w = 0);
		virtual ~AssistantClient();
		
		virtual void retranslate();
		
		inline Assistant* assistant() { return pOwner; }
		
		static QString docPath();
		
	public slots:
		void openLink(const QString& lnk);
		void openInNewTab(const QString& lnk);
		
	protected:
		virtual void contextMenuEvent(QContextMenuEvent *e);
		
	private slots:
		void openLink();
		void openInNewTab();
		
		void highlighted(const QString& link);
		void sourceChanged(const QUrl& url);
		void currentIndexChanged(const QString& s);
		
	private:
		QMenu *pMenu;
		Assistant *pOwner;
		
		QAction *aForward, *aBackward, *aReload, *aHome,
				*aOpenLink, *aOpenInTab;
		
		QLabel *pLabel;
		QComboBox *pUrl;
		QTextBrowser *pBrowser;
		
		QString anchor;
};


#endif
