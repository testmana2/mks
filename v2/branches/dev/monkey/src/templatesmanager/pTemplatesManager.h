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

#include <QApplication>
#include <QStringList>
#include <QComboBox>

class UITemplatesWizard;
class pTemplatePreviewer;
class ProjectItem;

struct Q_MONKEY_EXPORT pTemplate
{
	QString Language;
	QString Type;
	QString Name;
	QString Description;
	QIcon Icon;
	QString DirPath; //with '/' at end
	QStringList FileNames;
	QString ProjectType;  // name of Project plugin, just for projects
	
	QHash <QString,QStringList> TextVariables;
	
	/*
	Will be displayed in the ComboBox. 
	First value are default. Result will be returned in the TextVariables hash
	*/
	QHash <QString,QStringList> SelectableVariables;
	
	/*
	Key - name of file in the template dir
	Value - Name of text variable, that will be used as new name
	Values of variables, used there, can't be empty. (Controled by UI)
	*/
	QHash <QString, QString> FilesForRename;
};

class Q_MONKEY_EXPORT pTemplatesManager : public QSingleton
{
	
public:
	//return all availible types of templates ( "Files", "Projects" ...)
	QStringList getTemplateTypes ();

	//Can translate some known strings
	const QString translate(QString);

	QString Name;
	QString Author;
	QString License;
	ProjectItem* Project;
	QString FileName;
	QString Comment;
	QString Content;

	/*Templates, that are provided by Monkey Developer Studio team
	(Templates from standard dirrecotry, for example /usr/share/monkey/templates)
	*/
	const QList<pTemplate> standardTemplates();
	//Templates from other locations
	const QList<pTemplate> availableTemplates();

	QStringList userTemplatesPath; //fuck off incapsulating and useless methods

	void setTemplatesHeader( const QString& );
	const QString templatesHeader();

	const QString tokenize( QString );
	const QString unTokenize( QString );
	
	void realiseTemplate (pTemplate)
protected:
	const QList<pTemplate> getTemplatesFromDir (QString);
	QHash <QString, QString> translations;
};
#endif // PTEMPLATESMANAGER_H
