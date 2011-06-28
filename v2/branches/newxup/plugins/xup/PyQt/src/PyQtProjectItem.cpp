#include "PyQtProjectItem.h"
#include "src/gui/UIPyQtEditor.h"

#include <CLIToolPlugin.h>
#include <UIMain.h>
#include <PluginsManager.h>

#include <QDir>
#include <QDebug>

PyQtProjectItem::PyQtProjectItem()
	: XUPProjectItem()
{
}

QString PyQtProjectItem::projectType() const
{
	return PLUGIN_NAME;
}

void PyQtProjectItem::installCommands()
{
	// get plugins
	CLIToolPlugin* ip = interpreter();
	
	// temp command
	pCommand cmd;

	// build command
	if ( ip ) {
		cmd = ip->command();
	}
	
	//cmd.setUserData( QVariant::fromValue( &mCommands ) );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	const pCommand cmdInterpret = cmd;
	
	// get qt version
	QString mainFile = relativeFilePath( attribute( "MAIN_FILE" ) );
	
	if ( mainFile.isEmpty() ) {
		mainFile = relativeFilePath( findFile( "main.py" ).value( 0 ).absoluteFilePath() );
	}
	
	// available commands
	if ( ip ) {
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

bool PyQtProjectItem::edit()
{
	return UIPyQtEditor( this, MonkeyCore::mainWindow() ).exec() == QDialog::Accepted;
}

QStringList PyQtProjectItem::filteredVariables() const
{
	const DocumentFilterMap filters = sourceFileNamePatterns();
	QMap<int, QString> variables;
	
	foreach ( const QString& name, filters.keys() ) {
		const DocumentFilter& filter = filters[ name ];
		variables.insertMulti( filter.weight, name );
	}
	
	return variables.values();
}

DocumentFilterMap PyQtProjectItem::sourceFileNamePatterns() const
{
	DocumentFilterMap filters;
	int weight = 0;
	
	filters[ "FORMS" ].weight = weight++;
	filters[ "FORMS" ].label = tr( "Qt Forms" );
	filters[ "FORMS" ].icon = "forms.png";
	filters[ "FORMS" ].filters << "*.ui";
	
	filters[ "PYTHON_FILES" ].weight = weight++;
	filters[ "PYTHON_FILES" ].label = tr( "Python Files" );
	filters[ "PYTHON_FILES" ].icon = "python.png";
	filters[ "PYTHON_FILES" ].filters << "*.py*";
	
	return filters;
}

CLIToolPlugin* PyQtProjectItem::interpreter() const
{
	return MonkeyCore::pluginsManager()->plugin<CLIToolPlugin*>( PluginsManager::stAll, "Python" );
}
