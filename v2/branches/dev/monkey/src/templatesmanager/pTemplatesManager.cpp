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
						QHash <QString,QStringList> (),
					   };
	
	QStringList variables = set.value("Variables").toStringList();
	foreach (QString var, variables)
		templ.Variables.insert(var, set.value(var).toStringList());
	
    return templ;
}

void pTemplatesManager::setTemplate (pTemplate t)
{
	qWarning () << "Saving template" << t.DirPath+"/"+ t.Name+ "/"+"template.ini";
	QSettings set (t.DirPath+"/"+ t.Name+ "/"+"template.ini", QSettings::IniFormat);
	if (set.status() != QSettings::NoError)
	{
		qWarning ()<<"Error reading file "<<  + "/template.ini "<<"Ignored";
		return ;
	}
	
	set.setValue ("Language", QVariant(t.Language));
	set.setValue ("Type", QVariant(t.Type));
	set.setValue ("Description", QVariant(t.Description));
	set.setValue ("Script", QVariant(t.Script));
	set.setValue ("Icon", QVariant(t.Icon));
	set.setValue ("Files", QVariant(t.Files));
	set.setValue ("Variables", QVariant (t.Variables.keys()));
	foreach (QString var, t.Variables.keys())
		set.setValue (var, t.Variables[var]);
}

TemplateList pTemplatesManager::getTemplates()
{
	TemplateList result;
	foreach (QString dir, getTemplatesPath ())
		foreach (QString templateName, getTemplatesNames (dir))
			result << getTemplate (dir +"/" + templateName);
	return result;
}

bool pTemplatesManager::realiseTemplate (pTemplate t, VariablesManager::Dictionary dict)
{
	QString dest = dict["Destination"];
	if ( (! QDir(dest).exists ()) || dest.isEmpty())
	{
		QMessageBox::information (NULL, tr("Error"), tr ("Not exist dirrectory '%1'").arg(dict["Destination"]));
		return false;
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
