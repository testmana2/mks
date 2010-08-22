#include "PHPQtProjectItem.h"
#include "PluginsManager.h"
#include "XUPPlugin.h"

#include <ProjectTypesIndex.h>
#include <pMonkeyStudio.h>
#include <CLIToolPlugin.h>
#include <UIMain.h>

#include "src/gui/UIPHPEditor.h"

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

QString PHPQtProjectItem::projectType() const
{
	return PROJECT_TYPE_STRING;
}

XUPProjectItem* PHPQtProjectItem::newProject() const
{
	return new PHPQtProjectItem();
}

QStringList PHPQtProjectItem::autoActivatePlugins() const
{
	return QStringList("PHP");
}

void PHPQtProjectItem::installCommands()
{
	// get plugins
	CLIToolPlugin* ip = interpreter();
	
	// temp command
	pCommand cmd;

	// build command
	if ( ip )
	{
		cmd = ip->command();
	}
	
	//cmd.setUserData( QVariant::fromValue( &mCommands ) );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	const pCommand cmdInterpret = cmd;
	
	// get qt version
	QString mainFile = relativeFilePath( attribute( "MAIN_FILE" ) );
	
	if ( mainFile.isEmpty() )
	{
		mainFile = relativeFilePath( findFile( "main.php" ).value( 0 ).absoluteFilePath() );
	}
	
	// available commands
	if ( ip )
	{
		// execute project
		cmd = cmdInterpret;
		cmd.setText( tr( "Start" ) );
		cmd.setArguments( mainFile );
		cmd.setParsers( QStringList() );
		cmd.setTryAllParsers( false );
		addCommand( cmd, "mInterpreter" );
		
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}

QStringList PHPQtProjectItem::sourceFiles() const
{
	QStringList result;
	XUPItem* sf = getVariable(this, "PHP_FILES");
	if (NULL == sf)
	{
		qWarning() << "PHP_FILES variable not found. Incorrect project\n";
		return QStringList();
	}
	
	foreach(XUPItem* file, sf->childrenList())
	{
		if (XUPItem::File != file->type())
		{
			qWarning() << "File is not a file";
		}
		result << file->content();
	}
	
	return result;
}

void PHPQtProjectItem::addFiles( const QStringList& files, XUPItem* scope )
{
	XUPProjectItem* project = scope->project();
	
	foreach (QString file, files)
	{
		XUPItem* var = NULL;
		QString varName;
		if (file.endsWith(".ui"))
		{
			varName = "FORMS";
		}
		else if (file.endsWith(".php"))
		{
			varName = "PHP_FILES";
		}
		
		if (varName.isNull())
		{
			setLastError("Don't know how to add file " + file);
			return;
		}
		
		var = project->getVariable( scope, varName );
		if (NULL == var)
		{
			var = scope->addChild( XUPItem::Variable );
			var->setAttribute( "name", varName );
			
		}
		
		XUPItem* value = var->addChild( XUPItem::File );
		value->setContent( project->relativeFilePath( file ) );
	}
}

void PHPQtProjectItem::removeItem( XUPItem* item )
{
	if (item->type() != XUPItem::File)
	{
		qWarning() << "Atempt to delete non-file item\n";
		return;
	}
	
	item->parent()->removeChild(item);
}

bool PHPQtProjectItem::edit()
{
	return UIPHPEditor( this, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

CLIToolPlugin* PHPQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "PHP" );
}

StringStringListList PHPQtProjectItem::sourceFileNamePatterns() const
{
	return StringStringListList()
		<< qMakePair( QString( "Forms" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "PHP files" ), QStringList( "*.php*" ) );
}

QStringList PHPQtProjectItem::filteredVariables() const
{
	return QStringList( "FORMS" ) << "PHP_FILES";
}

QString PHPQtProjectItem::iconsPath() const
{
	return ":/phpqtitems";
}

QString PHPQtProjectItem::variableDisplayText( const QString& variableName ) const
{
	if ("FORMS" == variableName)
		return tr( "Qt Forms" );
	else if ("PHP_FILES" == variableName)
		return tr( "PHP files" );
	else
		return variableName;
}

QString PHPQtProjectItem::variableDisplayIcon( const QString& variableName ) const
{
	if ("FORMS" == variableName)
		return "forms.png";
	else if ("PHP_FILES" == variableName)
		return "php.png";
	else
		return QString::null;
}
