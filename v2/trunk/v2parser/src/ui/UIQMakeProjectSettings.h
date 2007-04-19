#ifndef UIQMAKEPROJECTSETTINGS_H
#define UIQMAKEPROJECTSETTINGS_H
//
#include "ui_UIQMakeProjectSettings.h"
//
#include <QApplication>
#include <QList>
#include <QHash>
//
class QMakeProjectScopesProxy;
class QMakeProjectModel;
//
struct QtItem
{
	enum Roles { ValueRole = Qt::UserRole, VariableRole };
	QtItem( const QString& t, const QString& v, const QString& s, const QString& d = QString::null )
		: Text( t ), Value( v ), Variable( s ), Description( d )
	{}
	QString Text;
	QString Value;
	QString Variable;
	QString Description;
};
typedef QList<QtItem*> QtItemList;
typedef QHash<QString, QStringList> QtSettings; // scope|operator|variable, values
//
class UIQMakeProjectSettings : public QDialog, public Ui::UIQMakeProjectSettings
{
	Q_OBJECT
	//
public:
	static void execute( QMakeProjectModel*, QWidget* = QApplication::activeWindow() );
	~UIQMakeProjectSettings();
	QModelIndex currentIndex();
	void setCurrentIndex( const QModelIndex& );
	//
protected:
	UIQMakeProjectSettings( QMakeProjectModel*, QWidget* = 0 );
	//
	QMakeProjectScopesProxy* mProxy;
	QMakeProjectModel* mProject;
	QtItemList mModules;
	QtItemList mConfigs;
	QtSettings mSettings;
	//
protected slots:
	void loadModules();
	void loadConfigs();
	void loadSettings();
	void tb_clicked();
	void lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* );
	
	void cb_highlighted( int );
	void on_cbScopes_currentIndexChanged( const QString& );
	void on_cbOperators_currentIndexChanged( const QString& );
	void on_cbVariables_currentIndexChanged( const QString& );
	
	void on_tvScopes_clicked( const QModelIndex& );
	void on_tbAdd_clicked();
	void on_tbEdit_clicked();
	void on_tbRemove_clicked();
	void on_tbClear_clicked();
	void on_tbUp_clicked();
	void on_tbDown_clicked();
	virtual void accept();
	virtual void reject();
	//
};
//
#endif // UIQMAKEPROJECTSETTINGS_H
