/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Kopats Andrei aka hlamer <hlamer@tut.by>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : FileBrouserSettings.cpp
** Date      : 2007-11-04T22:50:45
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "FileBrowserSettings.h"
#include "FileBrowser.h"
#include "pDockFileBrowser.h"

FileBrowserSettings::FileBrowserSettings ()
{
	QVBoxLayout* vbox = new QVBoxLayout (this);

	list = new pStringListEditor (this, tr("Except filenames"));
	list->setValues ( pDockFileBrowser::instance()->getFilterWildCards () );
	
	vbox->addWidget (list);

	QHBoxLayout* applyBox = new QHBoxLayout (this);
	QPushButton* applyBtn = new QPushButton (tr("Apply"), this);
	applyBox->addWidget (applyBtn, 0, Qt::AlignRight);

	vbox->addLayout (applyBox);
	
	connect ( applyBtn, SIGNAL ( clicked()), this, SLOT (setSettings()));
}

FileBrowserSettings::~FileBrowserSettings ()
{
}

void FileBrowserSettings::setSettings()
{
	pDockFileBrowser::instance()->setFilterWildCards ( list->values() );
}
