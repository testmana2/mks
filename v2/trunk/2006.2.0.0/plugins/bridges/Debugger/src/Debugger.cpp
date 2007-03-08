#include "Debugger.h"
#include "MonkeyEditor.h"
//
Debugger::~Debugger()
{
	if ( isInstalled() )
		uninstall();
}
//
void Debugger::initialize( Workspace* w )
{
	BasePlugin::initialize( w );
	// plugin infos
	mPluginInfos.Caption = tr( "Debugger Bridge" );
	mPluginInfos.Description = tr( "This plugin provide a generic debugger bridge" );
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name ="Debugger";
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Installed = false;
}
//
bool Debugger::install()
{
	// plugin is installed
	mPluginInfos.Installed = true;
	return true;
}
//
bool Debugger::uninstall()
{
	// plugin is not installed
	mPluginInfos.Installed = false;
	return true;
}
//
//
void Debugger::registerMarkersEditor( MonkeyEditor* e )
{
	mEditors << e;
	e->markerDefine( MonkeyEditor::Circle, mBreakPoint );
	e->markerDefine( MonkeyEditor::RightArrow, mPointAt );
}
//
void Debugger::toggleBreakPoint( MonkeyEditor* e, int l, bool f )
{
	if ( !e )
		return;
	if ( !mEditors.contains( e ) )
		registerMarkersEditor( e );
	bool b = false;
	emit canChangeBreakPoint( &b );
	if ( b )
	{
		if ( e->markersAtLine( l ) & mBreakPoint )
		{
			if ( !f )
				e->markerDelete( l,  mBreakPoint );
		}
		else 
			e->markerAdd( l,  mBreakPoint );
	}
}
//













// editor is register
void Debugger::onEditorOpened( MonkeyEditor* e )// ,QString fileName, int atLine)
{
	emit signalEditorOpened( e );
}

// the editor request a breakpoint in fileName at line
void Debugger::onMarginClicked( MonkeyEditor* e, const QString& f, int l )
{
	emit signalMarginClicked( e, f, l );
}
//
void Debugger::onSetMarker( MonkeyEditor* e, const QPixmap& p, int m )
{
	emit signalSetMarker( e, p, m );
}
// for add marker in margin
void Debugger::onAddMarker( MonkeyEditor* e, int l, int m )
{
	emit signalAddMarker( e, l, m );
}
//
void Debugger::onAddMarker( const QString& f, int l, int m )
{
	emit signalAddMarker( f, l, m );
}
// for delete marker in margin
void Debugger::onDelMarker( MonkeyEditor* e, int l, int m )
{
	emit signalDelMarker( e, l, m );
}
// for delete all marker in margin
void Debugger::onDelAllMarker( MonkeyEditor * e, int m )
{
	emit signalDelAllMarker( e, m );
}
// for delete all marker in margin
void Debugger::onDelAllMarker( const QString& f, int m )
{
	emit signalDelAllMarker( f, m );
}
//
void Debugger::onGotoLine( const QString& f, int l )
{	
	emit signalGotoLine( f, l );
}
//
void Debugger::saveCodeCoverage(const QString &name)
{
  QString testname=name+"/"+infos().Name;
#ifdef __COVERAGESCANNER__
  __coveragescanner_filename(codeCoverageFile().toAscii());
  __coveragescanner_testname(testname.toAscii());
  __coveragescanner_save();
#endif
}
//
Q_EXPORT_PLUGIN2( BaseDebugger, Debugger )
