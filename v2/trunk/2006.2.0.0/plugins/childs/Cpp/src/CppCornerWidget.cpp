#include "CppCornerWidget.h"
#include "CppChild.h"
#include "AbstractProjectProxy.h"
#include "Workspace.h"
//
#include <QHBoxLayout>
#include <QActionGroup>
#include <QToolButton>
//
QPointer<CppCornerWidget> CppCornerWidget::mSelf = 0L;
//
CppCornerWidget* CppCornerWidget::self( QWidget* p )
{
	if ( !mSelf )
		mSelf = new CppCornerWidget( p );
	return mSelf;
}
//
CppCornerWidget::CppCornerWidget( QWidget* p )
	: QWidget( p ), mChild( 0L )
{
	// set icon
	setCursor( Qt::PointingHandCursor );
	// layout
	QHBoxLayout* mLayout = new QHBoxLayout( this );
	mLayout->setMargin( 1 );
	mLayout->setSpacing( 1 );
	// actiongroup
	QActionGroup* ag = new QActionGroup( this );
	QToolButton* tb;
	// form
	tb = new QToolButton( this );
	tb->setIconSize( QSize( 14, 14 ) );
	aForm = new QAction( QIcon( ":/Icons/Icons/form.png" ), QString::null, tb );
	aForm->setShortcut( tr( "Ctrl+Alt+F" ) );
	aForm->setToolTip( tr( "Form" ) );
	tb->setDefaultAction( aForm );
	mLayout->addWidget( tb );
	// header
	tb = new QToolButton( this );
	tb->setIconSize( QSize( 14, 14 ) );
	aHeader = ag->addAction( QIcon( ":/Icons/Icons/h.png" ), QString::null );
	aHeader->setShortcut( tr( "Ctrl+Alt+H" ) );
	aHeader->setToolTip( tr( "Header" ) );
	aHeader->setCheckable( true );
	tb->setDefaultAction( aHeader );
	mLayout->addWidget( tb );
	// source
	tb = new QToolButton( this );
	tb->setIconSize( QSize( 14, 14 ) );
	aSource = ag->addAction( QIcon( ":/Icons/Icons/cpp.png" ), QString::null );
	aSource->setShortcut( tr( "Ctrl+Alt+C" ) );
	aSource->setToolTip( tr( "Source" ) );
	aSource->setCheckable( true );
	tb->setDefaultAction( aSource );
	mLayout->addWidget( tb );
	//
	connect( aForm, SIGNAL( triggered() ), this, SLOT( aForm_triggered() ) );
	connect( ag, SIGNAL( triggered( QAction* ) ), this, SLOT( ag_triggered( QAction* ) ) );
}
//
void CppCornerWidget::setChild( CppChild* c )
{
	mChild = c;
	setEnabled( c );
	if ( !mChild )
		return;
	if ( mChild->currentEditor() == mChild->mHeader )
		aHeader->setChecked( true );
	else if ( mChild->currentEditor() == mChild->mSource )
		aSource->setChecked( true );
}
//
void CppCornerWidget::aForm_triggered()
{
	if ( !mChild )
		return;
	// get all path to check in
	Workspace* w = Workspace::self();
	QStringList l;
	QString s;
	QFileInfo f( mChild->currentFile() );
	//
	if ( mChild->proxy() )
	{
		// check inf FORMS variable
		//i = l.indexOf( QRegExp( QString( "*%1*" ).arg( f.baseName() ), Qt::CaseSensitive, QRegExp::Wildcard ) );
		l = mChild->proxy()->project()->getValuesList( "FORMS" ).filter( f.baseName() );	
		if ( l.count() )
		{
			w->openFile( mChild->proxy()->project()->filePath( l.at( 0 ) ), mChild->proxy() );
			return;
		}
		// check inf FORMS3 variable
		l = mChild->proxy()->project()->getValuesList( "FORMS3" ).filter( f.baseName() );	
		if ( l.count() )
		{
			w->openFile( mChild->proxy()->project()->filePath( l.at( 0 ) ), mChild->proxy() );
			return;
		}
		// checking in all path that project include
		l << mChild->proxy()->project()->getValuesList( "INCLUDEPATH" );
		l << mChild->proxy()->project()->getValuesList( "DEPENDPATH" );
		l << mChild->proxy()->project()->getValuesList( "VPATH" );
		// made path absolute
		for ( int i = 0; i < l.count(); i++ )
		{
			QFileInfo fs( l.at( i ) );
			if ( fs.isRelative() )
				l[i] = mChild->proxy()->project()->filePath( l.at( i ) );
			l[i] = QFileInfo( l.at( i ) ).canonicalFilePath();
		}
	}
	// loop all path
	foreach ( QString ds, l )
	{
		// go to path
		QDir d( ds );
		// looping all files
		foreach ( QString file, d.entryList( QStringList( QString( "%1.*" ).arg( f.baseName() ) ), QDir::Files ) )
		{
			// we only need to open one
			if ( QFileInfo( file ).completeSuffix().toLower() == "ui" )
			{
				w->openFile( d.absoluteFilePath( file ), mChild->proxy() );
				return;
			}
		}
	}
}
//
void CppCornerWidget::ag_triggered( QAction* a )
{
	if ( !mChild )
		return;
	//
	if ( a == aHeader )
		mChild->mHeader->setFocus();
	else if ( a == aSource )
		mChild->mSource->setFocus();
	else
		return;
	mChild->layoutModeChanged( mChild->layoutMode() );
}
