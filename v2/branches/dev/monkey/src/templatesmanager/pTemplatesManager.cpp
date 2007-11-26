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
#include "pFileManager.h"

#include <QHash>
#include <QDebug>
#include <QMessageBox>

pTemplatesManager::pTemplatesManager ()
{}

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
						set.value ("Type","Wrong template type").toString(),
						set.value ("Description","No desctiption").toString(),
						set.value ("Script").toString(),
						set.value ("Icon").toString(), //just an empty icon, if not exist
						QFileInfo(d).filePath()+"/",
						set.value ("Files").toStringList(),
                        set.value ("FilesToOpen").toStringList(),
                        set.value ("ProjectsToOpen").toStringList(),
						QHash <QString,QStringList> (),
					   };
	
	QStringList variables = set.value("Variables").toStringList();
	foreach (QString var, variables)
		templ.Variables.insert(var, set.value(var+"Values").toStringList());
	
    return templ;
}

TemplateList pTemplatesManager::getTemplates()
{
	TemplateList result;
	foreach (QString dir1, getTemplatesPath ())
	{
		foreach (QString dir2, QDir(dir1).entryList (QDir::Dirs | QDir::NoDotAndDotDot))
		{
			foreach (QString templateName, getTemplatesNames (dir1+"/"+dir2))
				result << getTemplate (dir1+"/"+dir2 +"/" + templateName);
		}
	}
	return result;
}

bool pTemplatesManager::realiseTemplate (pTemplate t, VariablesManager::Dictionary dict)
{
	QString dest = dict["Destination"];
	if ( dest.isEmpty())
	{
		QMessageBox::information (NULL, tr("Error"), tr ("Not set dirrectory for template"));
		return false;
	}
	if ( ! QDir(dest).exists ())
	{
        if ( ! QDir().mkpath (dest))
        {
            QMessageBox::information (NULL, tr("Error"), tr ("Can't create dirrectory'%1'").arg(dict["Destination"]));
            return false;
        }
	}
	if ( ! dest.endsWith ("/"))
		dest += "/";
		
	QHash <QString,QString> files;
	qDebug () << t.Files;
	qDebug () << dict;
	foreach (QString f, t.Files)
	{
		QString newName = VariablesManager::instance()->replaceAllVariables (f,dict);
		if (newName.isEmpty())
		{
			QMessageBox::information (NULL, tr("Error"), tr("Empty filename detected for file ") + f);
			return false;
		}
		files[f] = newName;
	}
	
	foreach (QString f, files.keys())
	{
		QString newFile = dest+files[f];
		if ( ! QFile::copy (t.DirPath+f, newFile ))
		{
			QMessageBox::information (NULL, tr("Error"), tr("Can't copy '%1' to '%2'").arg(t.DirPath+f ).arg(newFile));
			return false;
		}
		QFile file (newFile);
		if ( ! file.open(QIODevice::ReadWrite))
		{
			QMessageBox::information(NULL, tr("Error"), tr ("Can't edit file %1: %2").arg(newFile).arg(file.errorString()));
			return false;
		}
		QString content = QString::fromLocal8Bit (file.readAll ());
		file.resize (0);
		file.write (VariablesManager::instance()->replaceAllVariables (content,dict).toLocal8Bit());
		file.close();
        if (t.FilesToOpen.contains (f))
            pFileManager::instance()->openFile (newFile);
        if (t.ProjectsToOpen.contains (f))
            pFileManager::instance()->openProject (newFile);
	}
	return true;
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
