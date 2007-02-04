#ifndef UITRANSLATOR_H
#define UITRANSLATOR_H
//
#include "ui_UITranslator.h"
#include "MonkeyExport.h"
//
#include <QPointer>
//
class Q_MONKEY_EXPORT UITranslator : public QDialog, public Ui::UITranslator
{
	Q_OBJECT
	//
public:
	static UITranslator* self( QWidget* = 0 );
	//
private:
	UITranslator( QWidget* = 0 );
	//
	static QPointer<UITranslator> mSelf;
	QString mTranslationsPath;
	//
public slots:
	virtual void accept();
	void on_tbLocate_clicked();
	void on_tbReload_clicked();
	//
};
//
#endif // UITRANSLATOR_H
