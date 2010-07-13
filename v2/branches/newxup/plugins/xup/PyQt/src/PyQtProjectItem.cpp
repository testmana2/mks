#include "PyQtProjectItem.h"
#include "PluginsManager.h"
#include "XUPPlugin.h"

#include <XUPProjectItemInfos.h>
#include <pMonkeyStudio.h>
#include <BuilderPlugin.h>
#include <InterpreterPlugin.h>

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

int PyQtProjectItem::projectType() const
{
	return PyQtProjectItem::PyQtProject;
}

void PyQtProjectItem::registerProjectType() const
{
	// get proejct type
	int pType = projectType();

	// register it
	mXUPProjectInfos->unRegisterType( pType );
	mXUPProjectInfos->registerType( pType, const_cast<PyQtProjectItem*>( this ) );

	// values
	const QString mPixmapsPath = ":/pyqtitems";
	const QStringList mOperators = QStringList( "=" );
	const QStringList mFilteredVariables = QStringList( "FORMS" ) << "PYTHON_FILES";
	const QStringList mFileVariables = QStringList( "FORMS" ) << "PYTHON_FILES";
	const QStringList mPathVariables = QStringList();
	const StringStringListList mSuffixes = StringStringListList()
		<< qMakePair( tr( "PyQt Project" ), QStringList( "*.xpyqt" ) );

	// register values
	mXUPProjectInfos->registerPixmapsPath( pType, mPixmapsPath );
	mXUPProjectInfos->registerOperators( pType, mOperators );
	mXUPProjectInfos->registerFilteredVariables( pType, mFilteredVariables );
	mXUPProjectInfos->registerFileVariables( pType, mFileVariables );
	mXUPProjectInfos->registerPathVariables( pType, mPathVariables );
	mXUPProjectInfos->registerSuffixes( pType, mSuffixes );
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
	InterpreterPlugin* ip = interpreter();
	
	// temp command
	pCommand cmd;

	// build command
	if ( ip )
	{
		cmd = ip->interpretCommand();
	}
	
	cmd.setUserData( QVariant::fromValue( &mCommands ) );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	const pCommand cmdInterpret = cmd;
	
	// get qt version
	QString mainFile = relativeFilePath( projectSettingsValue( "MAIN_FILE" ) );
	
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
	return mVariableCache.value( "PYTHON_FILES" ).split(" ");
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
		value->setAttribute( "content", project->relativeFilePath( file ) );
	}
}

XUPPlugin* PyQtProjectItem::editorPlugin()
{
	return MonkeyCore::pluginsManager()->plugins<XUPPlugin*>( PluginsManager::stAll, "PyQt" ).value( 0 );
}

InterpreterPlugin* PyQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<InterpreterPlugin*>( PluginsManager::stAll, "Python" );
}

StringStringListList PyQtProjectItem::sourceFileNamePatterns() const
{
	return StringStringListList()
		<< qMakePair( QString( "Qt Forms" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "Python files" ), QStringList( "*.py*" ) );
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
