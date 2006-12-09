/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : samedi 08 juillet 2006 - 15h09
 * AUTHOR       : Nox PasNox ( pasnox gmail.com )
 * FILENAME     : pDockStackedButton.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "pDockStackedButton.h"
#include "pStackedWidget.h"
//
#include <QResizeEvent>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
//
pDockStackedButton::pDockStackedButton( QWidget* p, Qt::Orientation m )
	: QDockWidget( p ), layout1( new QHBoxLayout ),
	layout2( new QVBoxLayout ), mStacked( new pStackedWidget ),
	mGroup( new QButtonGroup )
{
	QFrame* mFrame = new QFrame;
	setWidget( mFrame );
	mFrame->setFrameStyle( QFrame::StyledPanel | QFrame::Plain );
	mFrame->setLayout( layout1 );
	//
	layout1->setMargin( 3 );
	layout1->setSpacing( 3 );
	//
	layout2->setMargin( 0 );
	layout2->setSpacing( 0 );
	//
	setOrientation( m );
	//
	layout1->addLayout( layout2 );
	layout1->addWidget( mStacked, 1 );
	//
	layout1->addStretch();
	layout2->addStretch();
	mGroup.setExclusive( false );
	//
	mStacked->hide();
	//
	connect( &mGroup, SIGNAL( buttonClicked( int ) ), this, SLOT( buttonClicked( int ) ) );
	connect( mStacked, SIGNAL( currentChanged( int ) ), this, SIGNAL( currentChanged( int ) ) );
	connect( mStacked, SIGNAL( widgetRemoved( int ) ), this, SIGNAL( widgetRemoved( int ) ) );
	connect( mStacked, SIGNAL( signalShowEvent( QShowEvent* ) ), this, SLOT( stackedShowEvent( QShowEvent* ) ) );
	connect( mStacked, SIGNAL( signalHideEvent( QHideEvent* ) ), this, SLOT( stackedHideEvent( QHideEvent* ) ) );
}
//
pDockStackedButton::~pDockStackedButton()
{
}
//
void pDockStackedButton::stackedShowEvent( QShowEvent* e )
{
	QWidget* w = parentWidget();
	if ( !w )
		return;
	if ( mOrientation == Qt::Horizontal )
		setMaximumHeight( QWIDGETSIZE_MAX );
	else
		setMaximumWidth( QWIDGETSIZE_MAX );
}
//
void pDockStackedButton::stackedHideEvent( QHideEvent* e )
{
	QWidget* w = parentWidget();
	if ( !w )
		return;
	mSize = size();
	if ( mOrientation == Qt::Horizontal )
		setMaximumHeight( 56 );
	else
		setMaximumWidth( 32 );
}
//
void pDockStackedButton::addWidget( QWidget* w, const QString& s, const QPixmap& p )
{
	QToolButton* b = new QToolButton;
	b->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	b->setCheckable( true );
	if ( !p.isNull() )
		b->setIcon( QIcon( p ) );
	b->setText( makeTitle( s ) );
	b->setSizePolicy( mPolicy );
	mGroup.addButton( b, mGroup.buttons().count() );
	layout2->insertWidget( 1, b );
	mStacked->addWidget( w );
}
//
void pDockStackedButton::removeWidget( QWidget* w )
{
	delete mGroup.button( mStacked->indexOf( w ) );
	delete w;
}
//
QWidget* pDockStackedButton::currentWidget() const
{
	return mStacked->currentWidget();
}
//
QWidget* pDockStackedButton::widget( int i ) const
{
	return mStacked->widget( i );
}
//
int pDockStackedButton::currentIndex() const
{
	return mStacked->currentIndex();
}
//
int pDockStackedButton::indexOf( QWidget* w ) const
{
	return mStacked->indexOf( w );
}
//
int pDockStackedButton::count() const
{
	return mStacked->count();
}
//
Qt::Orientation pDockStackedButton::orientation() const
{
	return mOrientation;
}
//
QString pDockStackedButton::makeTitle( const QString& s )
{
	QString mTitle = s.simplified();
	if ( mOrientation == Qt::Vertical )
	{
		QStringList l;
		for ( int i = 0; i < mTitle.count(); i++ )
			l << mTitle.at( i ) << "\n";
		mTitle = l.join( QString::null );
	}
	return mTitle;
}
//
void pDockStackedButton::setCurrentWidget( QWidget* w )
{
	mGroup.button( mStacked->indexOf( w )  )->setChecked( true );
	buttonClicked( mStacked->indexOf( w )  );
}
//
void pDockStackedButton::setCurrentIndex( int i )
{
	mGroup.button( i  )->setChecked( true );
	buttonClicked( i  );
}
//
void pDockStackedButton::setOrientation( Qt::Orientation o )
{
	mOrientation = o;
	switch ( mOrientation )
	{
	case Qt::Vertical:
		layout1->setDirection( QBoxLayout::LeftToRight );
		layout2->setDirection( QBoxLayout::BottomToTop );
		mPolicy = QSizePolicy( QSizePolicy::Minimum, QSizePolicy::Preferred );
		break;
	case Qt::Horizontal:
		layout1->setDirection( QBoxLayout::BottomToTop );
		layout2->setDirection( QBoxLayout::RightToLeft );
		mPolicy = QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum );
		break;
	}
}
//
void pDockStackedButton::buttonClicked( int i )
{
	mStacked->setCurrentIndex( i );
	foreach ( QAbstractButton* b, mGroup.buttons() )
		if ( b != mGroup.button( i ) )
			b->setChecked( false );
	if ( mStacked->isVisible() != mGroup.button( i )->isChecked()  )
		mStacked->setVisible( mGroup.button( i )->isChecked() );
}
