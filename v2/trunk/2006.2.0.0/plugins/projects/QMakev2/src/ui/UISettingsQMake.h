#ifndef UISETTINGSQMAKE_H
#define UISETTINGSQMAKE_H
//
#include "ui_UISettingsQMake.h"
//
#include <QDir>
#include <QList>
//
class QsciLexerCPP;
class QsciAPIs;
//
struct QtItem
{
	enum Roles { ValueRole = Qt::UserRole, VariableRole, HelpRole };
	QtItem( const QString& t, const QString& v, const QString& s, const QString& h = QString::null )
		: Text( t ), Value( v ), Variable( s ), Help( h )
	{}
	QString Text;
	QString Value;
	QString Variable;
	QString Help;
};
typedef QList<QtItem*> QtItemList;
//
class UISettingsQMake : public QWidget, public Ui::UISettingsQMake
{
	Q_OBJECT
	//
public:
	enum Type { BinaryType, PathType };
	enum Role { TypeRole = Qt::UserRole, DataRole };
	UISettingsQMake( QWidget* = 0 );
	//
	void recursiveFiles( QDir );
	void generateApi( const QString&, const QString& );
	static QStringList defaultFilters();
	static QStringList readFilters();
	static QtItemList defaultQtModules();
	static QtItemList readQtModules();
	static QtItemList defaultSettings();
	static QtItemList readSettings();
	//
private:
	QsciLexerCPP* mLexer;
	QsciAPIs* mAPI;
	QString mFile;
	//
private slots:
	void generationFinished();
	void loadSettings();
	void on_cbKeywords_currentIndexChanged( int );
	void on_lePath_editingFinished();
	void on_tbBrowse_clicked();
	void on_pbGenerate_clicked();
	void lw_currentItemChanged( QListWidgetItem*, QListWidgetItem* );
	void tbAdd_clicked();
	void tbRemove_clicked();
	void tbClear_clicked();
	void on_bbDialog_clicked( QAbstractButton* );
	//
};
//
#endif // UISETTINGSQMAKE_H
