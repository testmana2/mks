#ifndef PSHORTCUTSEDITOR_H
#define PSHORTCUTSEDITOR_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include <QDialog>
#include <QHash>

class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;
class QLineEdit;

class Q_MONKEY_EXPORT pShortcutsEditor : public QDialog, public QSingleton<pShortcutsEditor>
{
	Q_OBJECT
	friend class QSingleton<pShortcutsEditor>;

public:
	pShortcutsEditor( QWidget* = 0 );

private:
	QHash<QString, QTreeWidgetItem*> mItems;
	QTreeWidget* twShortcuts;
	QPushButton* pbRestore;
	QPushButton* pbClear;
	QLineEdit* leShortcut;
	QPushButton* pbSet;
	QPushButton* pbClose;

private slots:
	void on_twShortcuts_itemSelectionChanged();
	void pbRestore_pbSet_clicked();
	void on_leShortcut_textChanged( const QString& );

};

#endif // PSHORTCUTSEDITOR_H
