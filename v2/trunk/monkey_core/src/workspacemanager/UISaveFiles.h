#ifndef UISAVEFILES_H
#define UISAVEFILES_H
//
#include <QDialog>
//
#include "MonkeyExport.h"
//
class AbstractChild;
class QCloseEvent;
class QListWidget;
class QAbstractButton;
class QDialogButtonBox;
//
class Q_MONKEY_EXPORT UISaveFiles : public QDialog
{
	Q_OBJECT
	//
public:
	static void execute( AbstractChild*, QCloseEvent* );
	//
private:
	UISaveFiles( AbstractChild* = 0, QCloseEvent* = 0 );
	void addFile( const QString&, bool );
	//
	AbstractChild* mChild;
	QCloseEvent* mEvent;
	QListWidget* lwFiles;
	QDialogButtonBox* dbbButtons;
	//
private slots:
	void clicked( QAbstractButton* );
	//
};
//
#endif // UISAVEFILES_H
