#ifndef PSHORTCUTSEDITOR_H
#define PSHORTCUTSEDITOR_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QDialog>

class QTreeWidget;
class QPushButton;
class QLineEdit;

class Q_MONKEY_EXPORT pShortcutsEditor : public QDialog, public QSingleton<pShortcutsEditor>
{
	Q_OBJECT
	friend class QSingleton<pShortcutsEditor>;

public:
	pShortcutsEditor( QWidget* = 0 );

private:
	QTreeWidget* twShortcuts;
	QPushButton* pbRestore;
	QPushButton* pbClear;
	QLineEdit* leShortcut;
	QPushButton* pbSet;
	QPushButton* pbClose;

private slots:
	void on_twShortcuts_itemSelectionChanged();
	void on_pbRestore_clicked();
	void on_leShortcut_textChanged( const QString& );
	void on_pbSet_clicked();

};

#endif // PSHORTCUTSEDITOR_H
