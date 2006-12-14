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
	static UIToolsEdit* self( QWidget* = 0 );
	struct Tool
	{
		QString Caption;
		QString iconFilePath;
		QString binFilePath;
	};
	//
private:
	UIToolsEdit( QWidget* = 0 );
	static QPointer<UIToolsEdit> mSelf;
	//
public slots:
	void on_lwTools_itemClicked( QListWidgetItem* );
	void on_pbNew_clicked();
	void on_pbDelete_clicked();
	void on_pbUp_clicked();
	void on_pbDown_clicked();
	void on_tbHelp_clicked();
	void on_leCaption_editingFinished();
	void on_tbIcon_clicked();
	void on_lePath_editingFinished();
	void accept();
	//
};
//
#endif // UITOOLSEDIT_H
