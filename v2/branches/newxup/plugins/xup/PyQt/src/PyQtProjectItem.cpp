#include "PyQtProjectItem.h"
#include "PluginsManager.h"
#include "XUPPlugin.h"

#include "src/gui/UIPyQtEditor.h"

#include <ProjectTypesIndex.h>
#include <pMonkeyStudio.h>
#include <CLIToolPlugin.h>
#include <UIMain.h>

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

QString PyQtProjectItem::projectType() const
{
	return PROJECT_TYPE_STRING;
}

XUPProjectItem* PyQtProjectItem::newProject() const
{
	return new PyQtProjectItem();
}

QStringList PyQtProjectItem::autoActivatePlugins() const
{
	return QStringList("Python");
}

void PyQtProjectItem::installCommands()
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
	
	cmd.setUserData( QVariant::fromValue( &mCommands ) );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	const pCommand cmdInterpret = cmd;
	
	// get qt version
	QString mainFile = relativeFilePath( attribute( "MAIN_FILE" ) );
	
	if ( mainFile.isEmpty() )
	{
		mainFile = relativeFilePath( findFile( "main.py" ).value( 0 ).absoluteFilePath() );
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

QStringList PyQtProjectItem::sourceFiles() const
{
	QStringList result;
	XUPItem* sf = getVariable(this, "PYTHON_FILES");
	if (NULL == sf)
	{
		qWarning() << "PYTHON_FILES variable not found. Incorrect project\n";
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

void PyQtProjectItem::addFiles( const QStringList& files, XUPItem* scope )
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
		else if (file.endsWith(".py"))
		{
			varName = "PYTHON_FILES";
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

void PyQtProjectItem::removeItem( XUPItem* item )
{
	if (item->type() != XUPItem::File)
	{
		qWarning() << "Atempt to delete non-file item\n";
		return;
	}
	
	item->parent()->removeChild(item);
}

bool PyQtProjectItem::edit()
{
	return UIPyQtEditor( this, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

CLIToolPlugin* PyQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "Python" );
}

StringStringListList PyQtProjectItem::sourceFileNamePatterns() const
{
	return StringStringListList()
		<< qMakePair( QString( "Qt Forms" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "Python files" ), QStringList( "*.py*" ) );
}

QStringList PyQtProjectItem::filteredVariables() const
{
	return QStringList( "FORMS" ) << "PYTHON_FILES";
}

QString PyQtProjectItem::iconsPath() const
{
	return ":/pyqtitems";
}

QString PyQtProjectItem::variableDisplayText( const QString& variableName ) const
{
	if ("FORMS" == variableName)
		return tr( "Qt Forms" );
	else if ("PYTHON_FILES" == variableName)
		return tr( "Python files" );
	else
		return variableName;
}

QString PyQtProjectItem::variableDisplayIcon( const QString& variableName ) const
{
	if ("FORMS" == variableName)
		return "forms.png";
	else if ("PYTHON_FILES" == variableName)
		return "python.png";
	else
		return QString::null;
}
