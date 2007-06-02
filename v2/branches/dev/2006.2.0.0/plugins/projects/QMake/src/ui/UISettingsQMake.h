#ifndef UISETTINGSQMAKE_H
#define UISETTINGSQMAKE_H
//
#include "ui_UISettingsQMake.h"
//
#include <QDir>
//
class QsciLexerCPP;
class QsciAPIs;
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
	void on_bbDialog_clicked( QAbstractButton* );
	//
};
//
#endif // UISETTINGSQMAKE_H
