#ifndef UITOOLSEDIT_H
#define UITOOLSEDIT_H
//
#include "ui_UIToolsEdit.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class Q_MONKEY_EXPORT UIToolsEdit : public QDialog, public Ui::UIToolsEdit
{
	Q_OBJECT
	//
public:
	enum ItemData { idCaption = Qt::DisplayRole, idFileIcon = Qt::UserRole +1, idFilePath, idWorkingPath };
	static UIToolsEdit* self( QWidget* = 0 );
	//
private:
	UIToolsEdit( QWidget* = 0 );
	static QPointer<UIToolsEdit> mSelf;
	//
protected:
	void closeEvent( QCloseEvent* );
	//
public slots:
	void on_lwTools_itemClicked( QListWidgetItem* );
	void on_pbNew_clicked();
	void on_pbDelete_clicked();
	void on_pbUp_clicked();
	void on_pbDown_clicked();
	void on_tbHelp_clicked();
	void on_leCaption_editingFinished();
	void on_tbFileIcon_clicked();
	void on_leFilePath_editingFinished();
	void on_tbFilePath_clicked();
	void on_tbUpdateWorkingPath_clicked();
	void on_leWorkingPath_editingFinished();
	void on_tbWorkingPath_clicked();
	void accept();
	//
};
//
#endif // UITOOLSEDIT_H
