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
	QtVersionManager* mQtManager;
	pGenericTableModel* mQtVersionsModel;
	
	void loadSettings();
	void goAtDocumentStart( QPlainTextEdit* pte );
	
	// qt versions
	void setQtVersion( const QModelIndex& versionIndex );
	void getQtVersion( const QModelIndex& versionIndex );
	void updateQtVersionState();

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
	
	void tbAdd_clicked();
	void tbRemove_clicked();
	void tbClear_clicked();
	void tbUp_clicked();
	void tbDown_clicked();
	//void on_tbDefaultQtVersion_clicked();
	void qtVersionChanged();
	void on_tbQtVersionPath_clicked();
	void on_tbQtVersionQMakeSpec_clicked();
	void lw_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	
	void on_dbbButtons_helpRequested();
	void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // UISETTINGSQMAKE_H
