#include "AStylePlugin.h"
#include "UISettingsAStyle.h"
#include "MenuBar.h"
#include "MonkeyEditor.h"
#include "astyle.h"
#include "FormatterSettings.h"
//
void AStylePlugin::initialize( Workspace* w )
{
	WorkspacePlugin::initialize( w );
	//
	mPluginInfos.Caption = tr( "AStyle" );
	mPluginInfos.Description = tr( "Uses AStyle to reformat your sources. Useful when copying code from the net or if you just want to reformat your sources based on a specific style." );
	mPluginInfos.Type = BasePlugin::iWorkspace;
	mPluginInfos.Name = "AStyle";
	mPluginInfos.Version = "1.0.0";
	mPluginInfos.Installed = false;
	//
	aAStyle = 0L;
};
//
QWidget* AStylePlugin::settingsWidget()
{
	return new UISettingsAStyle;
}
//
bool AStylePlugin::install()
{
	if ( isInstalled() )
		return true;
	// create action
	aAStyle = mWorkspace->menuBar()->action( "mEdit/aAStyle", tr( "AStyle Formatter" ), QIcon( ":/Icons/astyle.png" ), tr( "Ctrl+I" ), mPluginInfos.Description );
	// connect action
	connect( aAStyle, SIGNAL( triggered() ), this, SLOT( applyFormatter() ) );
	// set plugin installed
	mPluginInfos.Installed = true;
	return true;
};
//
bool AStylePlugin::uninstall()
{
	if ( !isInstalled() )
		return true;
	// disconnect action
	disconnect( aAStyle, SIGNAL( triggered() ), this, SLOT( applyFormatter() ) );
	// delete action
	delete aAStyle;
	aAStyle = 0L;
	// set plugin uninstalled
	mPluginInfos.Installed = false;
	return true;
};
//
void AStylePlugin::applyFormatter()
{
	if ( !mWorkspace->currentWidget() )
		return;
	MonkeyEditor* e = qobject_cast<MonkeyEditor*>( qApp->focusWidget() ); //mWorkspace->currentWidget()->focusWidget() );
	if ( e )
	{
		QString s1 = e->text();
		QString s2;
		//
		astyle::ASFormatter f;
		// load settings
		FormatterSettings s;
		s.applyTo( f );
		// eol
		QString eolChars;
		switch ( e->eolMode() )
		{
		case MonkeyEditor::EolWindows:
			eolChars = "\r\n";
			break;
		case MonkeyEditor::EolUnix:
			eolChars = "\n";
			break;
		case MonkeyEditor::EolMac:
			eolChars = "\r";
			break;
		}
		//
		if ( s1.length() && !s1.endsWith( '\r' ) && !s1.endsWith( '\n' ) )
			s1 += eolChars;
		//
		istringstream iter( s1.toStdString() );
		f.init( iter );
		while ( f.hasMoreLines() )
		{
			s2.append( f.nextLine().c_str() );
			if ( f.hasMoreLines() )
				s2.append( eolChars );
		}
		//
		int l, i;
		e->getCursorPosition(  &l, &i );
		e->clear();
		e->insert( s2 );
		e->setCursorPosition( l, i );
	}
}
//
void AStylePlugin::saveCodeCoverage(const QString &name)
{
  __coveragescanner_filename(codeCoverageFile().toAscii());
  QString testname=name+"/"+infos().Name;
#ifdef __COVERAGESCANNER__
  __coveragescanner_testname(testname.toAscii());
  __coveragescanner_save();
#else
  qDebug() << "No coverage Support ["+testname+"]";
#endif
}
//
Q_EXPORT_PLUGIN2( WorkspaceAStylePlugin, AStylePlugin )
