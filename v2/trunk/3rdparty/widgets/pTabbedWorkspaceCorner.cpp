#include "pTabbedWorkspaceCorner.h"
#include "pTabbedWorkspace.h"

#include <QToolButton>
#include <QPaintEvent>
#include <QPainter>

pTabbedWorkspaceCorner::pTabbedWorkspaceCorner( pTabbedWorkspace* w, const QSize& s )
	: QWidget( w ), mWorkspace( w ), mLayout( new QBoxLayout( QBoxLayout::LeftToRight, this ) ), mSize( s )
{
	/*
	QBoxLayout::LeftToRight
	QBoxLayout::RightToLeft
	QBoxLayout::TopToBottom
	QBoxLayout::BottomToTop
	*/

	mLayout->setSpacing( 3 );
	mLayout->setMargin( 0 );
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
			{
				QPainter p( tb );
				switch ( mLayout->direction() )
				{
				case QBoxLayout::LeftToRight:
				case QBoxLayout::RightToLeft:
					break;
				case QBoxLayout::TopToBottom:
				case QBoxLayout::BottomToTop:
					p.rotate( 90 );
					bool b = QWidget::eventFilter( o, e );
					return b;
					break;
				}
			}
		}
	}
	// return default filter event
	return QWidget::eventFilter( o, e );
}

QBoxLayout::Direction pTabbedWorkspaceCorner::direction() const
{
	return mLayout->direction();
}

void pTabbedWorkspaceCorner::clearActions()
{
	qDeleteAll( findChildren<QToolButton*>() );
}

void pTabbedWorkspaceCorner::setButtonSize( const QSize& s )
{
	mSize = s;
	foreach ( QToolButton* tb, findChildren<QToolButton*>() )
		tb->setFixedSize( mSize );
}

void pTabbedWorkspaceCorner::setDirection( QBoxLayout::Direction d )
{
	mLayout->setDirection( d );
	switch ( mLayout->direction() )
	{
	case QBoxLayout::LeftToRight:
	case QBoxLayout::RightToLeft:
		mSize.transpose();
		break;
	case QBoxLayout::TopToBottom:
	case QBoxLayout::BottomToTop:
		mSize.transpose();
		break;
	}
	setButtonSize( mSize );
}

void pTabbedWorkspaceCorner::addAction( QAction* a )
{
	QToolButton* tb = new QToolButton( this );
	tb->installEventFilter( this );
	tb->setFixedSize( mSize );
	tb->setDefaultAction( a );
	mLayout->addWidget( tb );
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
