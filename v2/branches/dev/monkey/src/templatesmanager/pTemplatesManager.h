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

class UITemplatesWizard;
class pTemplatePreviewer;

struct Q_MONKEY_EXPORT pTemplate
{
	enum TemplateType { ttUnknow = 0, ttFiles, ttProjects, ttExtras };
	pTemplate() {}
	pTemplate( const QString& l, TemplateType t, const QString& n, const QString& d, const QString& i, const QStringList& f )
		: Language( l ), Type( t ), Name( n ), Description( d ), Icon( i ), FileNames( f ) {}

	QString Language;
	TemplateType Type;
	QString Name;
	QString Description;
	QString Icon;
	QStringList FileNames;
};

struct Q_MONKEY_EXPORT pTemplateContent
{
	QString Name;
	QString Author;
	QString License;
	QString Project;
	QString FileName;
	QString Comment;
	QString Content;
};

namespace pTemplatesManager
{
	const QList<pTemplate> defaultTemplates();
	const QList<pTemplate> templates();

	void setTemplatesPath( const QString& );
	const QString templatesPath();

	void setTemplatesHeader( const QString& );
	const QString templatesHeader();

	const QString tokenize( QString );
	const QString unTokenize( QString );

	const QStringList tokenize( QStringList );
	const QStringList unTokenize( QStringList );

	const QString processContent( pTemplateContent );
};

#endif // PTEMPLATESMANAGER_H
