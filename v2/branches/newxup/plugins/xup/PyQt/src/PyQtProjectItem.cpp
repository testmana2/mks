#include "PyQtProjectItem.h"
#include "PluginsManager.h"
#include "XUPPlugin.h"

#include <XUPProjectItemInfos.h>
#include <pMonkeyStudio.h>
#include <BuilderPlugin.h>
#include <InterpreterPlugin.h>
#include <pCommand.h>

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
	const StringStringList mVariableLabels = StringStringList()
		<< qMakePair( QString( "FORMS" ), tr( "Qt Forms" ) )
		<< qMakePair( QString( "PYTHON_FILES" ), tr( "Python Files" ) );
	const StringStringList mVariableIcons = StringStringList()
		<< qMakePair( QString( "FORMS" ), QString( "forms" ) )
		<< qMakePair( QString( "PYTHON_FILES" ), QString( "python" ) );

	// Variable suffixes
	const StringStringListList mSourceFileNamePatterns = StringStringListList()
		<< qMakePair( QString( "Qt Forms" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "Python files" ), QStringList( "*.py*" ) );

	// register values
	mXUPProjectInfos->registerPixmapsPath( pType, mPixmapsPath );
	mXUPProjectInfos->registerOperators( pType, mOperators );
	mXUPProjectInfos->registerFilteredVariables( pType, mFilteredVariables );
	mXUPProjectInfos->registerFileVariables( pType, mFileVariables );
	mXUPProjectInfos->registerPathVariables( pType, mPathVariables );
	mXUPProjectInfos->registerSuffixes( pType, mSuffixes );
	mXUPProjectInfos->registerVariableLabels( pType, mVariableLabels );
	mXUPProjectInfos->registerVariableIcons( pType, mVariableIcons );
	mXUPProjectInfos->registerSourceFileNamePatterns( pType, mSourceFileNamePatterns );
}

XUPProjectItem* PyQtProjectItem::newProject() const
{
	return new PyQtProjectItem();
}

InterpreterPlugin* PyQtProjectItem::interpreter( const QString& plugin ) const
{
	QString plug = plugin;

	if ( plug.isEmpty() )
	{
		plug = "Python";
	}

	return XUPProjectItem::interpreter( plug );
}

void PyQtProjectItem::installCommands()
{
	mInterpretAction = MonkeyCore::menuBar()->action("mInterpreter/aInterpret", QObject::tr("Run project"));
	connect(mInterpretAction, SIGNAL(triggered()), this, SLOT(on_interpret_clicked()));
}

void PyQtProjectItem::uninstallCommands()
{
	delete mInterpretAction;
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

void PyQtProjectItem::on_interpret_clicked(void)
{
	// get plugins
	InterpreterPlugin* ip = interpreter();
	
	if ( !ip )
		return;
	
	QString mainFile = relativeFilePath( projectSettingsValue( "MAIN_FILE" ) );
	if ( mainFile.isEmpty() )
	{
		mainFile = relativeFilePath( findFile( "main.py" ).value( 0 ).absoluteFilePath() );
	}
	
	// TODO 1: if there are only one file - take it
	// TODO 2: ask user for file, save the choise
	
	if ( mainFile.isEmpty() )
		return;
	
	pCommand cmd = ip->interpretCommand();

	cmd.setProject( this );
	cmd.setSkipOnError( false );
	cmd.setText( tr( "Start" ) );
	cmd.setArguments( mainFile );
	cmd.setWorkingDirectory(path());
	
	MonkeyCore::consoleManager()->addCommand( cmd );
}
