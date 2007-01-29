#include "CppCornerWidget.h"
#include "CppChild.h"
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
	aForm->setToolTip( tr( "Form" ) );
	tb->setDefaultAction( aForm );
	mLayout->addWidget( tb );
	// header
	tb = new QToolButton( this );
	tb->setIconSize( QSize( 14, 14 ) );
	aHeader = ag->addAction( QIcon( ":/Icons/Icons/h.png" ), QString::null );
	aHeader->setToolTip( tr( "Header" ) );
	aHeader->setCheckable( true );
	tb->setDefaultAction( aHeader );
	mLayout->addWidget( tb );
	// source
	tb = new QToolButton( this );
	tb->setIconSize( QSize( 14, 14 ) );
	aSource = ag->addAction( QIcon( ":/Icons/Icons/cpp.png" ), QString::null );
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
	qWarning( "CppCornerWidget::aForm_triggered() not yet implemented." );
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
