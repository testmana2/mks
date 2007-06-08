#ifndef PTABBEDWORKSPACECORNERBUTTON_H
#define PTABBEDWORKSPACECORNERBUTTON_H

#include "MonkeyExport.h"

#include <QToolButton>
#include <QBoxLayout>

class QStyleOptionToolButton;

class Q_MONKEY_EXPORT pTabbedWorkspaceCornerButton : public QToolButton
{
	Q_OBJECT

public:
	pTabbedWorkspaceCornerButton( QWidget* p, QBoxLayout::Direction = QBoxLayout::LeftToRight );

	virtual QSize sizeHint() const;

	QBoxLayout::Direction direction() const;

public slots:
	void setDirection( QBoxLayout::Direction );

protected:
	virtual void paintEvent( QPaintEvent* );
	virtual void mousePressEvent( QMouseEvent* );
	virtual void mouseReleaseEvent( QMouseEvent* );

	bool hitArrow( bool = true ) const; // bool check button state
	QMenu* hasMenu() const;
	bool menuButtonDown() const;
	void setStyleOption( QStyleOptionToolButton* ) const;

	QBoxLayout::Direction mDirection;
	bool mMenuDown;

};

#endif // PTABBEDWORKSPACECORNERBUTTON_H
