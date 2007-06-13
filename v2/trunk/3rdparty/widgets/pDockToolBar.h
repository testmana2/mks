#ifndef PDOCKTOOLBAR_H
#define PDOCKTOOLBAR_H

#include "MonkeyExport.h"

#include <QToolBar>
#include <QPixmap>
#include <QHash>

class QDockWidget;
class QAbstractButton;
class QButtonGroup;
class QFrame;
class QBoxLayout;
class pDockToolBarManager;

class Q_MONKEY_EXPORT pDockToolBar : public QToolBar
{
	Q_OBJECT

public:
	pDockToolBar( pDockToolBarManager*, Qt::Orientation = Qt::Horizontal );

	virtual bool eventFilter( QObject*, QEvent* );

	int addDock( QDockWidget*, const QString& = QString(), const QPixmap& = QPixmap() );
	void removeDock( QDockWidget* );
	void removeDock( int );
	bool isDockVisible( QDockWidget* ) const;
	bool isDockVisible( int ) const;
	int id( QDockWidget* ) const;
	QDockWidget* dock( int ) const;
	QDockWidget* dock( QAbstractButton* ) const;
	QList<QDockWidget*> docks() const;

	QAbstractButton* button( QDockWidget* ) const;
	QList<QAbstractButton*> buttons() const;
	int count() const;

	void checkVisibility();

private:
	pDockToolBarManager* mManager;
	int mUniqueId;
	QButtonGroup* mButtons;
	QFrame* mFrame;
	QBoxLayout* mLayout;
	QHash<int, QDockWidget*> mDocks;
	int mOldId;

private slots:
	void internal_orientationChanged( Qt::Orientation );
	void internal_dockChanged();
	void internal_dockDestroyed( QObject* );
	void internal_buttonClicked( int );

signals:
	void buttonClicked( int );
	void dockWidgetAreaChanged( QDockWidget*, pDockToolBar* );

};

#endif // PDOCKTOOLBAR_H
