#ifndef DEBUGGER_H
#define DEBUGGER_H
//
#include "BasePlugin.h"
//
class MonkeyEditor;
//
class Debugger : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )
	//
public:
	enum Markers { mBreakPoint = 1, mPointAt };
	//
	virtual ~Debugger();
	virtual void initialize( Workspace* );
	virtual bool install();
	virtual bool uninstall();
	//
protected:
	QList<MonkeyEditor*> mEditors;
	//
public slots:
	// nox slots
	void registerMarkersEditor( MonkeyEditor* );
	void toggleBreakPoint( MonkeyEditor*, int, bool );
	// xiantia
	void onEditorOpened( MonkeyEditor* );
	void onSetMarker( MonkeyEditor* , const QPixmap&, int );
	void onMarginClicked( MonkeyEditor*, const QString&, int );
	void onAddMarker( MonkeyEditor*, int, int );
	void onAddMarker( const QString&, int, int );
	void onDelMarker( MonkeyEditor*, int, int );
	void onDelAllMarker( MonkeyEditor*, int );
	void onDelAllMarker( const QString&, int );
	void onGotoLine( const QString&, int );
	//
signals :
	// nox
	void canChangeBreakPoint( bool );
	// xiantia
	void signalEditorOpened( MonkeyEditor* );
	void signalSetMarker( MonkeyEditor*, const QPixmap&, int );
	void signalMarginClicked( MonkeyEditor*, const QString&, int );
	void signalAddMarker( MonkeyEditor*, int, int );
	void signalAddMarker( const QString&, int, int );
	void signalDelMarker( MonkeyEditor*, int, int );
	void signalDelAllMarker( MonkeyEditor * ,int );
	void signalDelAllMarker( const QString&,int );
	void signalGotoLine( const QString&, int );
	//
};
//
#endif // DEBUGGER_H
