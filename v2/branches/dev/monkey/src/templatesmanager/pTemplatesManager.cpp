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

TemplateList pTemplatesManager::getTemplatesFromDir (QString d)
{
    QDir dirrectory (d);
    QList<pTemplate> result;
    if ( !dirrectory.exists())
    {
        qWarning ()<<"Dirrectory not exist: "<<d;
        return result;
    }
    dirrectory.setFilter (QDir::Dirs|QDir::NoDotAndDotDot);
    foreach (QFileInfo subdir, dirrectory.entryInfoList())
    {
		qWarning () <<"Checking dirrectory" << subdir.absolutePath();
        if (!QFileInfo (subdir.absolutePath()+QString("/template.ini")).exists())
        {
            qWarning () <<"Not exist 'template.ini' file in the "<<subdir.absoluteDir();
        }
        QSettings set (subdir.absolutePath()+"/template.ini", QSettings::IniFormat);
        if (set.status() != QSettings::NoError)
        {
            qWarning ()<<"Error reading file "<<subdir.absolutePath()+"/template.ini "<<"Ignored";
            break;
        }
        pTemplate templ = { subdir.fileName (),
                            set.value ("Language","Other").toString(),
                            set.value ("Type","Wrong template path").toString(),
                            set.value ("Desctiption","No desctiption").toString(),
                            QIcon (subdir.absolutePath()+'/'+set.value ("Icon").toString()), //just an empty icon, if not exist
                            subdir.absolutePath(),
                            set.value ("FileNames").toStringList(),
                            set.value ("ProjectType").toString(),
                            QHash <QString,QStringList> (),
                            QHash <QString,QStringList> ()};
        QStringList textVariables = set.value ("TextVariables").toStringList();
        foreach (QString var, textVariables)
            templ.TextVariables.insert(var, set.value(var).toStringList());
        
        QStringList selectableVariables = set.value("SelecableVariables").toStringList();
        foreach (QString var, selectableVariables)
            templ.SelectableVariables.insert(var, set.value(var).toStringList());
        
        result.append (templ);
    }
    return result;
}

TemplateList pTemplatesManager::getTemplates()
{
	TemplateList result;
	foreach (QString dir, getTemplatesPath ())
		result << getTemplatesFromDir (dir);
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
