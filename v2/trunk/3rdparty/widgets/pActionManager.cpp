#include "pActionManager.h"
#include "pActionGroup.h"
#include "pAction.h"
#include "pShortcutsEditor.h"

#include <QSettings>
#include <QApplication>

pActionManager::pActionManager( QObject* o )
	: QObject( o ), mSettings( 0L )
{
}

void pActionManager::setSettings( QSettings* s, bool b )
{
	if ( mSettings == s )
		return;

	mSettings = s;
	if ( b )
		reloadSettings();
}

QSettings* pActionManager::settings() const
{
	return mSettings;
}

void pActionManager::reloadSettings()
{
	if ( !mSettings )
		return;

	foreach ( pActionGroup* ag, actionGroups )
	{
		foreach ( QAction* a, ag->actions() )
			a->setShortcut( QKeySequence( mSettings->value( QString( "MainWindow/Shortcuts/%1/%2" ).arg( ag->objectName() ).arg( a->objectName() ), a->property( "Default Shortcut" ) ).toString() ) );
	}
}

void pActionManager::addGroup( pActionGroup* g )
{
	actionGroups.append( g );
}

void pActionManager::removeGroup( pActionGroup* g )
{
	actionGroups.removeAll( g );
}

QKeySequence pActionManager::getShortCut( const QString& on, const QString& an, const QKeySequence& sc )
{
	if ( !instance()->settings() )
		return QKeySequence();

	return QKeySequence( instance()->settings()->value( QString( "MainWindow/Shortcuts/%1/%2" ).arg( on ).arg( an ), sc ).toString() );
}

void pActionManager::showSettings()
{
	pShortcutsEditor::instance( QApplication::activeWindow() )->exec();
}

QString pActionManager::setShortcutForAction( pAction* action, const QKeySequence& sc )
{
	bool existWithSameShortcut = false;
	QString s;

	foreach ( pActionGroup* ag, actionGroups )
	{
		foreach ( QAction* a, ag->actions() )
		{
			if ( a != action && a->shortcut() != sc && !sc.isEmpty() ) // != QKeySequence()
			{
				existWithSameShortcut = true;
				s = QString( "%1/%2" ).arg( ag->property( "Caption" ).toString() ).arg( a->text() );
				break;
			}
		}
		if ( existWithSameShortcut )
			break;
	}

	if ( existWithSameShortcut )
	{
		qDebug( qPrintable( QString( "Action with shortcut %1 already exist(%2)" ).arg( sc.toString() ).arg( s ) ) );
		return s;
	}
	else
	{
		action->setShortcut( sc );
		return "OK";
	}
}
