/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 01h14
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pMenuBar.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class allow you creating a menu bar using path for menus & actions, ie :
	For creating action File->Open simply use :
		class->menu( "mFile", tr( "&File" ) );
		class->action( "mFile/aOpen" );
	For creating menu File->Recents simply use :
		class->menu( "mFile", tr( "&File" ) );
		class->menu( "mFile/mRecents", tr( "&Recents" ) );
	This class use some modified code ( group members ) from Trolltech SA (c) QSettings.
 ********************************************************************************************************/
#include "pMenuBar.h"
//
QPointer<pMenuBar> pMenuBar::mSelf = 0L;
//
pMenuBar* pMenuBar::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new pMenuBar( p );
	return mSelf;
}
//
pMenuBar::pMenuBar( QWidget* p )
	: QMenuBar( p )
{
}
//
pMenuBar::~pMenuBar()
{
}
//
QAction* pMenuBar::searchAction( QMenu* m, const QString& s )
{
	QList<QAction*> mActions = m->findChildren<QAction*>();
	foreach ( QAction* mAction, mActions )
	{
		if ( mAction->objectName().toLower() == s.toLower() )
			return mAction;
	}
	if ( s.contains( QRegExp( tr( "^aseparator\\d{1,2}$" ), Qt::CaseInsensitive ) ) )
		return m->addSeparator();
	//
	QAction* a = m->addAction( QString::null );
	a->setObjectName( s );
	return a;
}
//
QString pMenuBar::normalizedKey( const QString& s )
{
	QString r = s;
	//
	int i = 0;
	while ( i < r.size() )
	{
		while ( r.at( i ) == QLatin1Char( '/' ) )
		{
			r.remove( i, 1 );
			if ( i == r.size() )
				goto after_loop;
		}
		while ( r.at( i ) != QLatin1Char( '/' ) )
		{
			++i;
			if ( i == r.size() )
				return r;
		}
		++i; // leave the slash alone
	}
	//
after_loop:
	if ( !r.isEmpty() )
		r.truncate( i -1 ); // remove the trailing slash
	return r;
}
//
void pMenuBar::beginGroupOrArray( const pMenuBarGroup& g )
{
	groupStack.push( g );
	if ( !g.name().isEmpty() )
	{
		groupPrefix += g.name();
		groupPrefix += QLatin1Char( '/' );
	}
}
//
void pMenuBar::beginGroup( const QString& s )
{
	beginGroupOrArray( pMenuBarGroup( normalizedKey( s ) ) );
}
//
QString pMenuBar::group() const
{
	return groupPrefix.left( groupPrefix.size() -1 );
}
//
void pMenuBar::endGroup()
{
	if ( groupStack.isEmpty() )
	{
		qWarning( "pMenuBar::endGroup: No matching beginGroup()" );
		return;
	}
	//
	pMenuBarGroup g = groupStack.pop();
	int len = g.toString().size();
	if ( len > 0 )
		groupPrefix.truncate( groupPrefix.size() -( len +1 ) );
	//
	if ( g.isArray() )
		qWarning( "pMenuBar::endGroup: Expected endArray() instead" );
}
//
QString pMenuBar::fixedPath( const QString& s, bool b )
{
	QString mString = s;
	if ( b )
		mString.prepend( groupPrefix );
	mString.replace( '\\', '/' );
	if ( mString.endsWith( '/' ) )
		mString.chop( 1 );
	return mString;
}
/*
	s: path
	i: icon
	c: shortcut
	t: statustip
	l: label
*/
QAction* pMenuBar::action( const QString& s, const QString& l, const QIcon& i, const QString& c, const QString& t )
{
	QString mString = fixedPath( s );
	QString mText = mString.mid( mString.lastIndexOf( '/' ) +1 );
	QString mPath;
	if ( mString.contains( '/' ) )
		mPath = mString.mid( 0, mString.lastIndexOf( '/' ) );
	QAction* mAction = searchAction( menu( mPath ), mText );
	if ( !l.isEmpty() && mAction->text() != l )
		mAction->setText( l );
	if ( !i.isNull() && mAction->icon().serialNumber() != i.serialNumber() )
		mAction->setIcon( i );
	if ( !c.isEmpty() && mAction->shortcut() != c )
		mAction->setShortcut( c );
	if ( !t.isEmpty() && mAction->statusTip() != t )
		mAction->setStatusTip( t );
	return mAction;
}
//
QMenu* pMenuBar::menu( const QString& s, const QString& l, const QIcon& i )
{
	QString mString = fixedPath( s, true );
	QMenu* mMenu = 0L;
	if ( !mMenus.contains( mString ) )
	{
		QStringList mMenuList = mString.split( '/' );
		mString.clear();
		for ( int i = 0; i < mMenuList.count(); i++ )
		{
			mString += '/' + mMenuList[ i ];
			if ( mString[ 0 ] == '/' )
				mString.remove( 0, 1 );
			if ( !mMenus.contains( mString ) )
			{
				mMenus[ mString ] = !mString.contains( '/' ) ? addMenu( mMenuList[ i ] ) : mMenus[ mString.left( mString.lastIndexOf( '/' ) ) ]->addMenu( mMenuList[ i ] );
				mMenus[ mString ]->setTitle( mString );
			}
		}
	}
	mMenu = mMenus[ mString ];
	if ( !l.isEmpty() && mMenu->title() != l )
		mMenu->setTitle( l );
	if ( !i.isNull() && mMenu->icon().serialNumber() != i.serialNumber() )
		mMenu->setIcon( i );
	return mMenu;
}
//
void pMenuBar::clearMenu( const QString& s )
{
	QString mString = fixedPath( s, true );
	if ( mMenus.contains( mString ) )
		mMenus[ mString ]->clear();
}
//
void pMenuBar::deleteMenu( const QString& s )
{
	QString mString = fixedPath( s, true );
	if ( mMenus.contains( mString ) )
	{
		mMenus[ mString ]->clear();
		delete mMenus.take( mString );
	}
}
