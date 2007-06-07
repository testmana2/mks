#include "pTabbedWorkspaceCorner.h"
#include "pTabbedWorkspace.h"

#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionToolButton>

#include <QDebug>

pTabbedWorkspaceCorner::pTabbedWorkspaceCorner( pTabbedWorkspace* w )
	: QWidget( w ), mWorkspace( w ), mLayout( new QBoxLayout( QBoxLayout::LeftToRight, this ) )
{
	/*
	QBoxLayout::LeftToRight
	QBoxLayout::RightToLeft
	QBoxLayout::TopToBottom
	QBoxLayout::BottomToTop
	*/

	mLayout->setSpacing( 3 );
	mLayout->setMargin( 0 );
	resize( 0, 0 );
}

pTabbedWorkspaceCorner::~pTabbedWorkspaceCorner()
{
	clearActions();
}

bool pTabbedWorkspaceCorner::eventFilter( QObject* o, QEvent* e )
{
	// draw rotated button or not
	if ( e->type() == QEvent::Paint )
	{
		QToolButton* tb = qobject_cast<QToolButton*>( o );
		if ( tb )
		{
			QPaintEvent* pe = dynamic_cast<QPaintEvent*>( e );
			if ( pe )
				drawButton( tb, pe );
			return true;
		}
	}
	
	// return default filter event
	return QWidget::eventFilter( o, e );
}

void pTabbedWorkspaceCorner::drawButton( QToolButton* b, QPaintEvent* e )
{
	QPainter painter( b );
	
	QStyleOptionToolButton options;
	qobject_cast<pTabbedWorkspaceCornerButton*>( b )->getStyleOption( &options );
	
	QSize sh = QSize( b->size() );
	
	if ( mLayout->direction() == QBoxLayout::TopToBottom || mLayout->direction() == QBoxLayout::BottomToTop )
		sh.transpose();

	int h = mWorkspace->tabBar()->sizeHint().width();
	qWarning( "height: %d", h );
	if ( sh.height() > h )
		sh.setHeight( h );
		
qWarning() << sh;
	
	options.rect.setSize( sh );
	
	QPixmap pixmap( sh );
	pixmap.fill( QColor( 0, 0, 0, 0 ) );
	QPainter pixpainter( &pixmap );
	b->style()->drawComplexControl( QStyle::CC_ToolButton, &options, &pixpainter, b );
	
	switch ( mLayout->direction() )
	{
	case QBoxLayout::TopToBottom:
		painter.rotate( -90 );
		painter.drawPixmap( 1 -pixmap.width(), 0, pixmap );
		break;
	case QBoxLayout::BottomToTop:
		painter.rotate( 90 );
		painter.drawPixmap( 0, 1 -pixmap.height(), pixmap );
		break;
	default:
		painter.drawPixmap( 0, 0, pixmap );
		break;
	}
}

QBoxLayout::Direction pTabbedWorkspaceCorner::direction() const
{
	return mLayout->direction();
}

void pTabbedWorkspaceCorner::clearActions()
{
	qDeleteAll( findChildren<QToolButton*>() );
}

void pTabbedWorkspaceCorner::setDirection( QBoxLayout::Direction d )
{
	QBoxLayout::Direction cd = mLayout->direction();

	// resize corner
	QSize s = size().isNull() ? sizeHint() : size();

	if ( ( cd == QBoxLayout::LeftToRight || cd == QBoxLayout::RightToLeft ) &&
		( d != QBoxLayout::LeftToRight && d != QBoxLayout::RightToLeft ) )
		s.transpose();

	if ( ( cd == QBoxLayout::TopToBottom || cd == QBoxLayout::BottomToTop ) &&
		( d != QBoxLayout::TopToBottom && d != QBoxLayout::BottomToTop ) )
		s.transpose();

	//setFixedSize( s );
	
	// resize buttons
	foreach ( QToolButton* tb, findChildren<QToolButton*>() )
	{
		QSize ts = tb->size().isNull() ? tb->sizeHint() : tb->size();

		if ( ( cd == QBoxLayout::LeftToRight || cd == QBoxLayout::RightToLeft ) &&
			( d != QBoxLayout::LeftToRight && d != QBoxLayout::RightToLeft ) )
			ts.transpose();

		if ( ( cd == QBoxLayout::TopToBottom || cd == QBoxLayout::BottomToTop ) &&
			( d != QBoxLayout::TopToBottom && d != QBoxLayout::BottomToTop ) )
			ts.transpose();

		//tb->setFixedSize( ts );
	}

	mLayout->setDirection( d );
}

void pTabbedWorkspaceCorner::addAction( QAction* a )
{
	pTabbedWorkspaceCornerButton* tb = new pTabbedWorkspaceCornerButton( this );
	tb->installEventFilter( this );
	tb->setToolButtonStyle( Qt::ToolButtonTextOnly );
	tb->setDefaultAction( a );
	mLayout->addWidget( tb );
	setDirection( direction() );
}

void pTabbedWorkspaceCorner::setActions( QList<QAction*> l )
{
	// hide to avoid flickering
	hide();

	// delete all buttons
	clearActions();

	// create button associated with actions
	foreach ( QAction* a, l )
		addAction( a );
	
	// show corner
	show();
}
