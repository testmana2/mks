#ifndef UISETTINGSQMAKE_H
#define UISETTINGSQMAKE_H

#include "ui_UISettingsQMake.h"

class QtVersionManager;
class pGenericTableModel;

class UISettingsQMake : public QWidget, public Ui::UISettingsQMake
{
	Q_OBJECT
	
public:
	UISettingsQMake( QWidget* parent = 0 );

protected:
	QBrush mBackground;
	QBrush mForeground;
	QtVersionManager* mQtManager;
	pGenericTableModel* mQtVersionsModel;
	pGenericTableModel* mQtModulesModel;
	pGenericTableModel* mQtConfigurationsModel;
	
	void loadSettings();
	void goAtDocumentStart( QPlainTextEdit* pte );
	
	// qt versions
	void setQtVersion( const QModelIndex& index );
	void getQtVersion( const QModelIndex& index );
	void updateQtVersionState();
	
	// qt modules
	void setQtModule( const QModelIndex& index );
	void getQtModule( const QModelIndex& index );
	void updateQtModuleState();
	
	// qt configuration
	void setQtConfiguration( const QModelIndex& index );
	void getQtConfiguration( const QModelIndex& index );
	void updateQtConfigurationState();

protected slots:
	void on_lwPages_currentRowChanged( int row );
	
	// qt versions
	void lvQtVersions_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
	void on_tbAddQtVersion_clicked();
	void on_tbRemoveQtVersion_clicked();
	void on_tbClearQtVersions_clicked();
	void on_tbUpQtVersion_clicked();
	void on_tbDownQtVersion_clicked();
	void on_tbDefaultQtVersion_clicked();
	void on_tbQtVersionPath_clicked();
	void on_tbQtVersionQMakeSpec_clicked();
	
	// qt modules
	void lvQtModules_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
	void on_tbAddQtModule_clicked();
	void on_tbRemoveQtModule_clicked();
	void on_tbClearQtModules_clicked();
	void on_tbUpQtModule_clicked();
	void on_tbDownQtModule_clicked();
	
	// qt modules
	void lvQtConfigurations_selectionModel_selectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
	void on_tbAddQtConfiguration_clicked();
	void on_tbRemoveQtConfiguration_clicked();
	void on_tbClearQtConfigurations_clicked();
	void on_tbUpQtConfiguration_clicked();
	void on_tbDownQtConfiguration_clicked();
	
	void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // UISETTINGSQMAKE_H
