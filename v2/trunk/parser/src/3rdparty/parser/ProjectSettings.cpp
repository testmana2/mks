#include "ProjectSettings.h"
#include "ProjectItemModel.h"
#include <QHeaderView>
#include <QFileDialog>
//#include <QDirModel>
#include <iostream.h>

ProjectSettings::ProjectSettings(ProjectItemModel* model) : QDialog()
{
	setupUi(this);
	setAttribute( Qt::WA_DeleteOnClose );
	mModel = model;
	setWindowTitle(model->currentProjectName());
	//
	connect(gb_Qt, SIGNAL(clicked(bool)), this, SLOT(slot_QtClicked(bool)));
	connect(rb_release, SIGNAL(clicked(bool)), SLOT(slot_rbClicked(bool)));
	connect(rb_debug, SIGNAL(clicked(bool)), SLOT(slot_rbClicked(bool)));
	connect(rb_debug_and_release, SIGNAL(clicked(bool)), SLOT(slot_rbClicked(bool)));
	connect(rb_warn_on, SIGNAL(clicked(bool)), SLOT(slot_rbClicked(bool)));
	connect(rb_warn_off, SIGNAL(clicked(bool)), SLOT(slot_rbClicked(bool)));
	connect(cbTemplate, SIGNAL(currentIndexChanged(int)), SLOT(slot_changeTemplate(int)));
	//
	expWindow->setColumnCount(1);
	expWindow->setSortingEnabled ( false );
	expWindow->setItemsExpandable ( false );
	expWindow->header()->setMovable (false);
	expWindow->header()->setClickable(false);
	expWindow->setRootIsDecorated(false);

	currDir = QDir(mModel->currentProjectPath());
	initExpWindow();

	connect (expWindow, SIGNAL(itemActivated ( QTreeWidgetItem * , int )),
			 SLOT(fileDoubleClicked ( QTreeWidgetItem * , int )) );
	connect (cbIncludeFile, SIGNAL(activated(int)),
			 this, SLOT(initExpWindow(/*int*/)));
	//
	initialize();
}
//
void ProjectSettings::initialize()
{
	QCheckBox* tmpCheckBox;
	QStringList tmpStringList;
	// TEMPLATE
	cbTemplate->setCurrentIndex( cbTemplate->findText(  getSettingsList("TEMPLATE").join( "" ) ) );
	if ( cbTemplate->currentIndex() == -1 )
		cbTemplate->setCurrentIndex( 0 );
	// LANGUAGE
	cbLanguage->setCurrentIndex( cbLanguage->findText( getSettingsList("LANGUAGE").join( "" ) ) );
	if ( cbLanguage->currentIndex() == -1 )
		cbLanguage->setCurrentIndex( 0 );
	//CONFIG
	tmpStringList = getSettingsList("CONFIG");
	foreach ( QString v, tmpStringList )
	{
		if (v == "qt")
			gb_Qt->setChecked(true);
		else if (v == "release")
			rb_release->click();
		else if (v == "debug")
			rb_debug->click();
		else if (v == "debug_and_release")
			rb_debug_and_release->click();
		else if (v == "build_all")
			ch_build_all->setChecked(true);
		else if (v == "warn_on")
			rb_warn_on->click();
		else if (v == "warn_off")
			rb_warn_off->click();
		else if (v == "warn_off")
			ch_ordered->setChecked(true);
		else if	( tmpCheckBox = pageConfig->findChild<QCheckBox*>( "ch_" + v ) )
			tmpCheckBox->setChecked( true );
	}
	//QT
	if (gb_Qt->isChecked())
	{
		tmpStringList = getSettingsList("QT");
		foreach ( QString v, tmpStringList )
		{
			if	( tmpCheckBox = gb_Qt->findChild<QCheckBox*>( "ch_qt_" + v ) )
				tmpCheckBox->setChecked( true );
		}
	}
	//TARGET AUTHOR VERSION DESTDIR
	QStringList varList;
	QLineEdit* tmpLineEdit;
	varList<<"TARGET"<<"AUTHOR"<<"VERSION"<<"DESTDIR";
	foreach (QString v, varList)
	{
		tmpStringList = getSettingsList(v);
		if (tmpStringList.isEmpty())
			continue;
		if	( tmpLineEdit = gb_Target->findChild<QLineEdit*>( "le_" + QString(v).toLower() ) )
			tmpLineEdit->setText(tmpStringList.join(" "));
		else
			le_destdir->setText(tmpStringList.join(" "));
	}

	if (cbTemplate->currentIndex() == 2) //SUBDIRS
		setSubdirs(true);

	on_cbIncludeFile_currentIndexChanged( cbIncludeFile->currentText() );
}
//
void ProjectSettings::setSubdirs(bool isSubdirs)
{
	gb_Qt->setDisabled(isSubdirs);
	gbExtraLib->setDisabled(isSubdirs);
	le_target->setDisabled(isSubdirs);
	le_destdir->setDisabled(isSubdirs);
	tbDestDir->setDisabled(isSubdirs);
}
//
QStringList ProjectSettings::getSettingsList(const QString& variable)
{
	QStringList list;
	if (variable == "CONFIG")
		list<<"qt"<<"warn_on"<<"debug";
	else if (variable == "QT")
		list<<"core"<<"gui";
	QHash<QString, uint> hash = mModel->simpleSettings()[variable];
	QHash<QString, uint>::iterator it = hash.begin();
	while( it != hash.end() )
	{
		if (it.key() == "=")
		{
			list.clear();
			list<<mModel->getSettingsList((ProjectItem*)it.value());
		}
		else if (it.key() == "-=")
		{
			QStringList tmpList = mModel->getSettingsList((ProjectItem*)it.value());
			foreach(QString str, tmpList)
			{
				list.removeAll(str);
			}
		}
		else
			list<<mModel->getSettingsList((ProjectItem*)it.value());
		++it;
	}
	return list;
}
//
void ProjectSettings::setSettingsList(
	const QString& variable,
	const QStringList& list,
	QString op) //op used only for -=
{
	ProjectItem* item = 0;
	if (!op.isEmpty())
		item = (ProjectItem*)mModel->simpleSettings()[variable][op];
	else
	{
		QHash<QString, uint> hash = mModel->simpleSettings()[variable];
		QHash<QString, uint>::iterator it = hash.begin();
		while( it != hash.end() )
		{
			if (it.key() != "-=")
			{
				item = (ProjectItem*)it.value();
				// break; //no break - get the last
			}
			++it;
		}
	}
	//	cout <<qPrintable("setSettingsList item = "+QString::number((uint)item))<<endl;
	if (!item)
	{
		if (op.isEmpty())
		{
			QStringList lst;
			lst<<"TEMPLATE"<<"TARGET"<<"LANGUAGE"<<"AUTHOR"<<"VERSION";
			if (lst.contains(variable))
				op = "=";
			else
				op = "+=";
		}
		//		cout <<qPrintable(variable+op)<<endl;
		item = mModel->addNewItem(variable, op);
	}

	if (item)
		mModel->setSettingsList(item, list);
}
//
void ProjectSettings::slot_rbClicked(bool)
{
	QRadioButton* rb = qobject_cast<QRadioButton*>(sender());
	if (rb == rb_release || rb == rb_debug)
	{
		ch_build_all->setChecked(false);
		ch_build_all->setEnabled(false);
	}
	else if (rb == rb_debug_and_release)
	{
		ch_build_all->setChecked(true);
		ch_build_all->setEnabled(true);
	}
	else if (rb == rb_warn_on)
	{
		rb_warn_off->setChecked(false);
	}
	else if (rb == rb_warn_off)
	{
		rb_warn_on->setChecked(false);
	}
}
//
void ProjectSettings::slot_QtClicked(bool checked)
{
	gbExtraLib->setEnabled(checked);
	ch_uic3->setEnabled(checked);
}
//
void ProjectSettings::slot_changeTemplate(int index)
{
	switch (index)
	{
		case 0:
			ch_ordered->setEnabled(false);
			ch_windows->setEnabled(true);
			ch_console->setEnabled(true);
			ch_staticlib->setEnabled(false);
			ch_plugin->setEnabled(false);
			ch_dll->setEnabled(false);
			cbIncludeFile->setCurrentIndex(0);
			setSubdirs(false);
			break;
		case 1:
			ch_ordered->setEnabled(false);
			ch_windows->setEnabled(false);
			ch_console->setEnabled(false);
			ch_staticlib->setEnabled(true);
			ch_plugin->setEnabled(true);
			ch_dll->setEnabled(true);
			cbIncludeFile->setCurrentIndex(0);
			setSubdirs(false);
			break;
		case 2:
			ch_ordered->setEnabled(true);
			ch_windows->setEnabled(false);
			ch_console->setEnabled(false);
			ch_staticlib->setEnabled(false);
			ch_plugin->setEnabled(false);
			ch_dll->setEnabled(false);
			cbIncludeFile->setCurrentIndex(7);
			setSubdirs(true);
			break;
	}
}
//
QString ProjectSettings::relativeFilePath(const QString& filePath)
{
	//	cout <<qPrintable(mModel->currentProjectPath()+"\n"+filePath)<<endl;
	return QDir(mModel->currentProjectPath()).relativeFilePath(filePath);
}

//
void ProjectSettings::initExpWindow(/*int filterIndex*/)
{
	if (!currDir.exists())
		currDir = QDir(mModel->currentProjectPath());

	expWindow->setHeaderLabels(QStringList()<<currDir.absolutePath());
	expWindow->headerItem()->setTextAlignment(0,Qt::AlignRight);

	QFont font = QFont("Lucida Console");
	font.setPointSize(QApplication::font().pointSize()-1);

	expWindow->headerItem()->setFont(0,font);
	expWindow->headerItem()->setTextColor(0, QColor("#737373"));

	expWindow->clear();

	currDir.setFilter(QDir::AllEntries | QDir::NoSymLinks);
	currDir.setSorting(QDir::DirsFirst | QDir::Name);
	//currDir.setNameFilters(filter);

	QFileInfoList infoList = currDir.entryInfoList ();

	QTreeWidgetItem *twi;

	QFileInfoList::iterator it = infoList.begin();
	while (it != infoList.end())
	{
		if (!QString::compare((*it).fileName(),".")  ||
				(!QString::compare((*it).fileName(),"..") && currDir.isRoot()) )
		{
			++it;
			continue;
		}
		twi = new QTreeWidgetItem(expWindow, QStringList()<<(*it).fileName());
		twi->setToolTip(0,(*it).absoluteFilePath());
		twi->setTextAlignment ( 0, Qt::AlignLeft );

		if ((*it).isDir())
		{
			twi->setIcon ( 0, QIcon(QString::fromUtf8(":/Icons/Icons/folder.png")) );
		}
		else
		{
			QStringList suffixList;
			QString suffix = QFileInfo((*it).fileName()).suffix();
			suffixList<<"so"<<"a"<<"dll"<<"ts"<<"qrc"<<"def"<<"rc";
			if (suffixList.contains(suffix))
				twi->setBackground(0, QColor(221,255,202));  //light green
			if (suffix == "cpp")
				twi->setIcon ( 0, QIcon(QString::fromUtf8(":/Icons/Icons/cpp.png")) );
			else if (suffix == "h")
				twi->setIcon ( 0, QIcon(QString::fromUtf8(":/Icons/Icons/h.png")) );
			else if (suffix == "gz" || suffix == "tar")
				twi->setIcon ( 0, QIcon(QString::fromUtf8(":/Icons/Icons/tgz.png")) );
			else if ((*it).isExecutable())
				twi->setIcon ( 0, QIcon(QString::fromUtf8(":/Icons/Icons/make.png")) );
			else if (suffix == "ui")
				twi->setIcon ( 0, QIcon(QString::fromUtf8(":/Icons/Icons/designer.png")) );
			else
				twi->setIcon ( 0, QIcon(QString::fromUtf8(":/Icons/Icons/unknown.png")) );
		}
		++it;
	}
	infoList.clear();

	expWindow->resizeColumnToContents(0);
}
//
void ProjectSettings::fileDoubleClicked ( QTreeWidgetItem * item, int column )
{
	if (column) return;

	bool cdDir = false;

	QFileInfo info (item->toolTip(0));

	if (QString::compare(item->text(0),"..") && info.isDir())
		cdDir = currDir.cd(currDir.absolutePath() + "/" + item->text(0));
	else if (!QString::compare(item->text(0),".."))
		cdDir = currDir.cdUp();

	if (cdDir)
	{
		initExpWindow(/*cbIncludeFile->currentIndex()*/);
		return;
	}
	QStringList suffixList;
	QString suffix = info.suffix();
	suffixList<<"so"<<"a"<<"dll"<<"ts"<<"qrc"<<"def"<<"rc";
	if (suffixList.contains(suffix))
		on_pbAdd_clicked();
}
//
void ProjectSettings::on_cbIncludeFile_currentIndexChanged( const QString& s )
{
	lwContents->clear();
	lwContents->addItems( getSettingsList(s) );
}
//
void ProjectSettings::on_pbAdd_clicked()
{
	bool needSave = false;
	QTreeWidgetItem* twi = expWindow->currentItem();
	if (!twi || twi->text(0) == "..")
		return;

	QFileInfo info(twi->toolTip(0));
	if (info.isDir())
	{
		switch (cbIncludeFile->currentIndex())
		{
			case 0: //LIBS
				{
					QString str = QString("-L%1").arg(relativeFilePath(info.absoluteFilePath()));
					lwContents->addItem(str);
					save_lwContents_changes();
					break;
				}
			case 1: // INCLUDEPATH
			case 4: // DEPENDPATH
			case 7: // SUBDIRS
				{
					QString str = relativeFilePath(info.absoluteFilePath());
					if (str.isEmpty())
						str = ".";
					lwContents->addItem(str);
					save_lwContents_changes();
					break;
				}
		}
	}
	else
	{
		QStringList suffixList;
		QString suffix = info.suffix();
		suffixList<<"so"<<"a"<<"dll"<<"ts"<<"qrc"<<"def"<<"rc";
		if (suffixList.contains(suffix))
		{
			if (suffix.indexOf(QRegExp("^(so|a)$")) != -1 )
			{
				if (cbIncludeFile->currentIndex() != 0)
					cbIncludeFile->setCurrentIndex(0);
				QString str = QString("-L%1").arg(relativeFilePath(info.absolutePath()));
				if (lwContents->findItems(str, Qt::MatchExactly).isEmpty())
					lwContents->addItem(str);
				str = "-l"+info.baseName().remove(QRegExp("^lib"));
				if (lwContents->findItems(str, Qt::MatchExactly).isEmpty())
					lwContents->addItem(str);
				save_lwContents_changes();
				return;
			}
			else
			{
				if (suffix == "dll" )
				{
					if (cbIncludeFile->currentIndex() != 0)
						cbIncludeFile->setCurrentIndex(0);
				}
				else if (suffix == "ts" )
				{
					if (cbIncludeFile->currentIndex() != 2)
						cbIncludeFile->setCurrentIndex(2);
				}
				else if (suffix == "qrc" )
				{
					if (cbIncludeFile->currentIndex() != 3)
						cbIncludeFile->setCurrentIndex(3);
				}
				else if (suffix == "def" )
				{
					if (cbIncludeFile->currentIndex() != 5)
						cbIncludeFile->setCurrentIndex(5);
				}
				else if (suffix == "rc" )
				{
					if (cbIncludeFile->currentIndex() != 6)
						cbIncludeFile->setCurrentIndex(6);
				}
			}
			QString text = relativeFilePath(info.absoluteFilePath());
			if (lwContents->findItems(text, Qt::MatchExactly).isEmpty())
				lwContents->addItem(text);
			save_lwContents_changes();
		} // if (suffixList.contains(suffix))
	}
}
//
void ProjectSettings::on_pbEdit_clicked()
{
	QListWidgetItem* lwi = lwContents->currentItem();
	if (!lwi)
		return;

	Qt::ItemFlags flags = lwi->flags();
	lwi->setFlags(flags|Qt::ItemIsEditable);
	lwContents->editItem(lwi);

	connect(lwContents, SIGNAL(itemChanged(QListWidgetItem*)),
			SLOT(itemTextChanged(QListWidgetItem*)));
	connect(lwContents, SIGNAL(itemActivated(QListWidgetItem*)),
			SLOT(itemTextChanged(QListWidgetItem*)));
}
//
void ProjectSettings::itemTextChanged(QListWidgetItem* item)
{
	QString text = item->text();
	if (text.simplified().isEmpty())
	{
		delete item;
		return;
	}
	//
	disconnect(lwContents, SIGNAL(itemChanged(QListWidgetItem*)),
			   this,  SLOT(itemTextChanged(QListWidgetItem*)));
	disconnect(lwContents, SIGNAL(itemActivated(QListWidgetItem*)),
			   this,  SLOT(itemTextChanged(QListWidgetItem*)));
	//
	item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
}
//
void ProjectSettings::on_pbDelete_clicked()
{
	QListWidgetItem* lwi = lwContents->currentItem();
	if (lwi)
		delete lwi;
	save_lwContents_changes();
}
//
void ProjectSettings::on_pbClear_clicked()
{
	lwContents->clear();
	save_lwContents_changes();
}
//
void ProjectSettings::save_lwContents_changes()
{
	QStringList l;
	QString v;
	for ( int i = 0; i < lwContents->count(); i++ )
	{
		v = lwContents->item( i )->text();
		if ( v.contains( " " ) && !v.contains( "\"" ) )
			v.prepend( "\"" ).append( "\"" );
		l << v;
	}
	//cout <<qPrintable("***"+l.join(" ")+"***")<<endl;
	setSettingsList(cbIncludeFile->currentText(), l);
}
//
void ProjectSettings::on_tbDestDir_clicked()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
				   currDir.path(),
				   QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!path.isEmpty())
		le_destdir->setText(relativeFilePath(path));
}