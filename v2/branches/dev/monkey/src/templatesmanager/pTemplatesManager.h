/*****************************************************
* AUTHOR   : Nox P@sNox
* PROJECT  : 
* FILENAME : pTemplatesManager.h
* DATE     : 2007/07/28
* TIME     : 15:17
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#ifndef PTEMPLATESMANAGER_H
#define PTEMPLATESMANAGER_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "VariablesManager.h"

#include <QApplication>
#include <QStringList>
#include <QComboBox>
#include <QStringList>

class UITemplatesWizard;
class pTemplatePreviewer;
class ProjectItem;

typedef QHash <QString,QStringList> VarList;

class Q_MONKEY_EXPORT pTemplate
{
public:
    QString Name;
    QString Language;
    QString Type;
    QString Description;
    QString Icon;
	QString Script;
    QString DirPath; //with '/' at end
    
    /*
    NOTE Filenames can contain variables. Example: Project$name$.pro
    This files will be renamed
    */
    QStringList Files;
    
    VarList Variables;

};

typedef QList<pTemplate> TemplateList;


class Q_MONKEY_EXPORT pTemplatesManager : public QObject, 
                                public QSingleton<pTemplatesManager>
{
    Q_OBJECT
    friend class QSingleton<pTemplatesManager>;
private:
    pTemplatesManager ();

public:
    //Can translate some known strings
    //const QString translate(QString);

    TemplateList getTemplates();

    //FIXME: remove this line and line below
    //I temporary removed headers processing. We must use some other way. 
    //Templates are not generic for various languages 
    //(or must be commented by language-specific comment)

    bool realiseTemplate (pTemplate, VariablesManager::Dictionary = VariablesManager::Dictionary());

    QStringList getTemplatesPath ();
	void setTemplatesPath (QStringList);

	//returns names of templates, that are on the path.
	QStringList getTemplatesNames (QString path);

    pTemplate getTemplate (QString);
    void setTemplate (pTemplate);
};

#endif // PTEMPLATESMANAGER_H
