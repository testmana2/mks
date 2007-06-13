#ifndef PDOCKTOOLBAR_H
#define PDOCKTOOLBAR_H

#include "MonkeyExport.h"

#include <QToolBar>
#include <QIcon>
#include <QHash>

class QDockWidget;
class QAbstractButton;
class QFrame;
class QBoxLayout;
class pDockToolBarManager;

class Q_MONKEY_EXPORT pDockToolBar : public QToolBar
{
	Q_OBJECT

public:
	pDockToolBar( pDockToolBarManager*, Qt::Orientation = Qt::Horizontal );

	virtual bool eventFilter( QObject*, QEvent* );

	int addDock( QDockWidget*, const QString& = QString(), const QIcon& = QIcon() );

	bool isDockVisible( int ) const;
	bool isDockVisible( QDockWidget* ) const;

	bool exclusive() const;

	int id( QDockWidget* ) const;
	int id( QAbstractButton* ) const;

	QDockWidget* dock( int ) const;
	QDockWidget* dock( QAbstractButton* ) const;

	QAbstractButton* button( int ) const;
	QAbstractButton* button( QDockWidget* ) const;
	
	QList<QDockWidget*> docks() const;
	QList<QAbstractButton*> buttons() const;

	int count() const;

private:
	pDockToolBarManager* mManager;
	int mUniqueId;
	bool mExclusive;
	QFrame* mFrame;
	QBoxLayout* mLayout;
	QHash<int, QAbstractButton*> mButtons;
	QHash<int, QDockWidget*> mDocks;

public slots:
	void removeDock( int );
	void removeDock( QDockWidget* );
	void setDockVisible( QDockWidget*, bool );
	void setExclusive( bool );

private slots:
	void internal_checkVisibility();
	void internal_checkButtonText( QAbstractButton* );
	void internal_orientationChanged( Qt::Orientation );
	void internal_dockChanged();
	void internal_dockDestroyed( QObject* );
	void internal_buttonClicked( bool );

signals:
	void buttonClicked( int );
	void dockWidgetAreaChanged( QDockWidget*, pDockToolBar* );

};

#endif // PDOCKTOOLBAR_H
