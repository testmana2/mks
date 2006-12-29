#ifndef UIPROJECTSMANAGER_H
#define UIPROJECTSMANAGER_H
//
#include "ui_UIProjectsManager.h"
//
#include <QPointer>
//
class AbstractProjectProxy;
//
class UIProjectsManager : public QWidget, public Ui::UIProjectsManager
{
	Q_OBJECT
	//
public:
	enum TreeItemRole { ProxyIdRole = Qt::UserRole, ProjectFilePathRole };
	//
	static UIProjectsManager* self( QWidget* = 0 );
	//
	void setCurrentProxy( AbstractProjectProxy* );
	AbstractProjectProxy* currentProxy() const;
	QTreeWidgetItem* getItemByProxyId( int );
	QAbstractItemView* getViewByProxyId( int );
	bool openProject( const QString&, int = -1 );
	void closeProject( QTreeWidgetItem* );
	//
private:
	UIProjectsManager( QWidget* = 0 );
	static QPointer<UIProjectsManager> mSelf;
	//
private slots:
	void on_tbOpen_clicked();
	void on_tbComplexCurrent_clicked();
	void on_tbSaveCurrent_clicked();
	void on_tbCloseCurrent_clicked();
	void on_twProjects_itemClicked( QTreeWidgetItem*, int );
	void on_swProjects_currentChanged( int );
	//
};
//
#endif // UIPROJECTSMANAGER_H
