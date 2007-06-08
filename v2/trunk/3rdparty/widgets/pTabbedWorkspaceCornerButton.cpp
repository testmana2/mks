#include "pTabbedWorkspaceCornerButton.h"

#include <QPaintEvent>
#include <QMouseEvent>
#include <QStyleOptionToolButton>
#include <QToolBar>
#include <QMenu>
#include <QPainter>

#include <QDebug>

pTabbedWorkspaceCornerButton::pTabbedWorkspaceCornerButton( QWidget* p, QBoxLayout::Direction d )
	: QToolButton( p ), mVisibleMenu( false )
{
	/*
	QBoxLayout::LeftToRight
	QBoxLayout::RightToLeft
	QBoxLayout::TopToBottom
	QBoxLayout::BottomToTop
	*/

	setDirection( d );
}

void pTabbedWorkspaceCornerButton::paintEvent( QPaintEvent* e )
{
qWarning( "paint" );
	// calcul angle rotation
	QSize s = QToolButton::sizeHint();
	int r = 0;
	QPoint p = QPoint();
	switch ( mDirection )
	{
	case QBoxLayout::TopToBottom:
		r = 90;
		p = QPoint( 0, -s.height() );
		break;
	case QBoxLayout::BottomToTop:
		r = -90;
		p = QPoint( -s.width(), 0 );
		break;
	default:
		break;
	}
	
	// if r = 0, do normal paintevent and return
	if ( r == 0 )
	{
		QToolButton::paintEvent( e );
		return;
	}

	// do rotated button
	QPixmap pixmap( s );
	pixmap.fill( QColor( 0, 0, 0, 0 ) );

	// get style options
	QStyleOptionToolButton o;
	setStyleOption( &o );

	// force to do horizontal paint
	o.rect.setSize( s );

	// draw button to pixmap
	QPainter pixpainter( &pixmap );
	style()->drawComplexControl( QStyle::CC_ToolButton, &o, &pixpainter, this );

	// draw pixmap on button
	QPainter painter( this );
	painter.rotate( r );
	painter.drawPixmap( p, pixmap );
}

void pTabbedWorkspaceCornerButton::mousePressEvent( QMouseEvent* e )
{
#ifndef QT_NO_MENU
	// test if we can check popup
	bool b = e->button() == Qt::LeftButton && popupMode() == QToolButton::MenuButtonPopup;

	if ( b )
	{
		// get bounding rectangle
		QRect r = rect();

		// get style options
		QStyleOptionToolButton opt;
		setStyleOption( &opt );

		// force to do horizontal calcul
		opt.rect.setSize( QToolButton::sizeHint() );
	
		// get arraow bounding rectangle
		QSize s = style()->subControlRect( QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButtonMenu, this ).size();
	
		// transpose for vertical
		s.transpose();
	
		// get valid bounding rectangle size
		r.setSize( s );

		// move top
		if ( mDirection == QBoxLayout::TopToBottom )
			r.moveTop( rect().height() -r.height() );

		// check menu show validity
		if ( r.isValid() && r.contains( e->pos() ) )
		{
			mVisibleMenu = true;
			showMenu();
			mVisibleMenu = false;
		}

		// update
		update();

		// return
		return;

	}

	// return default mousepressevent
	QToolButton::mousePressEvent( e );
#else
	// return default mousepressevent
	QToolButton::mousePressEvent( e );
#endif
}

void pTabbedWorkspaceCornerButton::mouseReleaseEvent( QMouseEvent* e )
{
	switch ( mDirection )
	{
	case QBoxLayout::TopToBottom:
	case QBoxLayout::BottomToTop:
		if ( hitButton( e->pos() ) && !mVisibleMenu )
		{
	update();
			click();
			
		}
		break;
	default:
		QToolButton::mouseReleaseEvent( e );
		break;
	}
}

QMenu* pTabbedWorkspaceCornerButton::hasMenu() const
{
	QMenu* m = menu();
	if ( !m && defaultAction() )
		m = defaultAction()->menu();
	return m;
}

bool pTabbedWorkspaceCornerButton::menuButtonDown() const
{
#ifndef QT_NO_MENU
	return hasMenu() && mVisibleMenu;
#else
	return false;
#endif
}

void pTabbedWorkspaceCornerButton::setStyleOption( QStyleOptionToolButton* option ) const
{
	if ( !option )
		return;

#if QT_VERSION >= 0x040300
	initStyleOption( o );
#else
	option->initFrom( this );
	bool forceNoText = false;
 
#ifndef QT_NO_TOOLBAR
	if ( parentWidget() )
	{
#ifdef QT3_SUPPORT
		if ( parentWidget()->inherits( "Q3ToolBar" ) )
		{
			int iconSize = style()->pixelMetric( QStyle::PM_ToolBarIconSize, option, this );
			option->iconSize = icon().actualSize( QSize( iconSize, iconSize ) );
			forceNoText = toolButtonStyle() == Qt::ToolButtonIconOnly;
		}
		else
#endif
		if ( QToolBar* toolBar = qobject_cast<QToolBar*>( parentWidget() ) )
		{
			option->iconSize = toolBar->iconSize();
		}
		else
		{
			option->iconSize = iconSize();
		}
	}
#endif // QT_NO_TOOLBAR
 
	if ( !forceNoText )
		option->text = text();
	option->icon = icon();
	option->arrowType = arrowType();
	if ( isDown() )
		option->state |= QStyle::State_Sunken;
	if ( isChecked() )
		option->state |= QStyle::State_On;
	if ( autoRaise() )
		option->state |= QStyle::State_AutoRaise;
	if ( !isChecked() && !isDown() )
		option->state |= QStyle::State_Raised;
 
	option->subControls = QStyle::SC_ToolButton;
	option->activeSubControls = QStyle::SC_None;
//	if ( isDown() && !menuButtonDown())
//		option->activeSubControls |= QStyle::SC_ToolButton;
 
	option->features = QStyleOptionToolButton::None;
	if ( popupMode() == QToolButton::MenuButtonPopup )
	{
		option->subControls |= QStyle::SC_ToolButtonMenu;
#if QT_VERSION >= 0x040300
		option->features |= QStyleOptionToolButton::MenuButtonPopup;
#endif
		if ( menuButtonDown() || isDown() )
		{
			option->state |= QStyle::State_MouseOver;
			option->activeSubControls |= QStyle::SC_ToolButtonMenu;
		}
	}
	else
	{
		if ( menuButtonDown() )
			option->state  |= QStyle::State_Sunken;
	}
	if ( arrowType() != Qt::NoArrow )
		option->features |= QStyleOptionToolButton::Arrow;
	if ( popupMode() == QToolButton::DelayedPopup )
		option->features |= QStyleOptionToolButton::PopupDelay;
#ifndef QT_NO_MENU
#if QT_VERSION >= 0x040300
	if ( hasMenu() )
		option->features |= QStyleOptionToolButton::HasMenu;
#else
	if ( hasMenu() )
		option->features |= QStyleOptionToolButton::Menu;
#endif
#endif
	option->toolButtonStyle = toolButtonStyle();
	if ( icon().isNull() && arrowType() == Qt::NoArrow && !forceNoText)
	{
		if ( !text().isEmpty() )
			option->toolButtonStyle = Qt::ToolButtonTextOnly;
		else if ( option->toolButtonStyle != Qt::ToolButtonTextOnly )
			option->toolButtonStyle = Qt::ToolButtonIconOnly;
	}
	else
	{
		if ( text().isEmpty() && option->toolButtonStyle != Qt::ToolButtonIconOnly )
			option->toolButtonStyle = Qt::ToolButtonIconOnly;
	}
 
	option->pos = pos();
	option->font = font();
#endif // QT_VERSION >= 0x040300
}

QSize pTabbedWorkspaceCornerButton::sizeHint() const
{
	//get default size
	QSize s = QToolButton::sizeHint();

	// calcul new size hint
	switch ( mDirection )
	{
	case QBoxLayout::LeftToRight:
	case QBoxLayout::RightToLeft:
		break;
	case QBoxLayout::TopToBottom:
	case QBoxLayout::BottomToTop:
		s.transpose();
		break;
	}

	// return new size hint;
	return s;
}

QBoxLayout::Direction pTabbedWorkspaceCornerButton::direction() const
{
	return mDirection;
}

void pTabbedWorkspaceCornerButton::setDirection( QBoxLayout::Direction d )
{
	if ( mDirection == d )
		return;
	mDirection = d;
	update();
}
