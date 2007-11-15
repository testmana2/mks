/*****************************************************
* AUTHOR   : Nox P@sNox
* PROJECT  : 
* FILENAME : pTemplatesManager.cpp
* DATE     : 2007/07/28
* TIME     : 15:17
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#include "pTemplatesManager.h"
#include "pMonkeyStudio.h"
#include "pSettings.h"
#include "ProjectItem.h"

#include <QDir>
#include <QHash>
#include <QDateTime>
#include <QDebug>

pTemplatesManager::pTemplatesManager ()
{
//    translations.insert ("Project", tr("Project"));
//    translations.insert ("File", tr("File"));
//    translations.insert ("Estra", tr("Extra"));
}

/*QString pTemplatesManager::translate(QString s)//TODO move to separate kernel class
{
    return translations.contains (s)? translations[s] : s;
}*/


QStringList pTemplatesManager::getTemplatesNames (QString path)
{
	QStringList result;
    QDir dirrectory (path);
    if ( !dirrectory.exists())
    {
        qWarning ()<<"Dirrectory not exist: "<<path;
        return result;
    }
    dirrectory.setFilter (QDir::Dirs|QDir::NoDotAndDotDot);
    foreach (QFileInfo subdir, dirrectory.entryInfoList())
		result.append (subdir.fileName ());

	return result;
}

pTemplate pTemplatesManager::getTemplate (QString d)
{
	QSettings set (d+"/template.ini", QSettings::IniFormat);
	if (set.status() != QSettings::NoError)
	{
		qWarning ()<<"Error reading file "<< d + "/template.ini "<<"Ignored";
		return pTemplate ();
	}
	pTemplate templ = { QFileInfo(d).fileName (),
						set.value ("Language","Other").toString(),
						set.value ("Type","Wrong template path").toString(),
						set.value ("Desctiption","No desctiption").toString(),
						set.value ("Script").toString(),
						set.value ("Icon").toString(), //just an empty icon, if not exist
						d,
						set.value ("Files").toStringList(),
						QHash <QString,QStringList> (),
					   };
	
	QStringList variables = set.value("Variables").toStringList();
	foreach (QString var, variables)
		templ.Variables.insert(var, set.value(var).toStringList());
	
    return templ;
}

void pTemplatesManager::setTemplate (pTemplate t)
{
	QSettings set (t.DirPath+"/", t.Name);
	if (set.status() != QSettings::NoError)
	{
		qWarning ()<<"Error reading file "<< d + "/template.ini "<<"Ignored";
		return ;
	}
	
	set.setValue ("Language", t
	pTemplate templ = { QFileInfo(d).fileName (),
						set.value ("Language","Other").toString(),
						set.value ("Type","Wrong template path").toString(),
						set.value ("Desctiption","No desctiption").toString(),
						set.value ("Script").toString(),
						set.value ("Icon").toString(), //just an empty icon, if not exist
						d,
						set.value ("Files").toStringList(),
						QHash <QString,QStringList> (),
					   };
	
	QStringList variables = set.value("Variables").toStringList();
	foreach (QString var, variables)
		templ.Variables.insert(var, set.value(var).toStringList());
}

TemplateList pTemplatesManager::getTemplates()
{
	TemplateList result;
	foreach (QString dir, getTemplatesPath ())
		foreach (QString templateName, getTemplatesNames (dir))
			result << getTemplate (dir +"/" + templateName);
	return result;
}

QStringList pTemplatesManager::getTemplatesPath ()
{
    return pSettings::instance()->value
				("Templates/TemplatePath", 
					QStringList(QApplication::applicationDirPath().append( "/../templates" ))
				).toStringList() ;
}

void pTemplatesManager::setTemplatesPath (QStringList path)
{
    pSettings::instance()->setValue("Templates/TemplatePath", QVariant(path));
}
