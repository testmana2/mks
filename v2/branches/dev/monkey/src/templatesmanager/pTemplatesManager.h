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

#include <QApplication>
#include <QStringList>
#include <QComboBox>

class UITemplatesWizard;
class pTemplatePreviewer;
class ProjectItem;

struct Q_MONKEY_EXPORT pTemplate
{
    QString Name;
    QString Language;
    QString Type;
    QString Description;
    QIcon Icon;
    QString DirPath; //with '/' at end
    
    /*
    NOTE Filenames can contain variables. Example: Project$name$.pro
    This files will be renamed
    */
    QStringList FileNames;
    QString ProjectType;  // name of Project plugin, just for projects
    
    QHash <QString,QStringList> TextVariables;
    
    /*
    Will be displayed in the ComboBox. 
    First value are default. Result will be returned in the TextVariables hash
    */
    QHash <QString,QStringList> SelectableVariables;
};

class Q_MONKEY_EXPORT pTemplatesManager : public QObject, 
                                public QSingleton<pTemplatesManager>
{
    
public:
    pTemplatesManager ();

    //Can translate some known strings
    //const QString translate(QString);

    const QList<pTemplate> getTemplates();

    //FIXME: remove this line and line below
    //I temporary removed headers processing. We must use some other way. 
    //Templates are not generic for various languages 
    //(or must be commented by language-specific comment)

    void realiseTemplate (pTemplate);
protected:

    const QList<pTemplate> getTemplatesFromDir (QString);
    //QHash <QString, QString> translations;
};
#endif // PTEMPLATESMANAGER_H
