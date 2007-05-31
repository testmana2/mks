#ifndef UISETTINGS_H
#define UISETTINGS_H
//
#include "ui_UISettings.h"
//
#include <QPointer>
//
class QsciLexer;
//
class UISettings : public QDialog, public Ui::UISettings
{
	Q_OBJECT
	//
public:
	static UISettings* self( QWidget* = 0 );
	//
	enum Designer { Embedded = 0, External };
	enum ExternalChanges { Nothing = 0, Alert, Reload };
	enum Lexer { Id = Qt::UserRole, EolFill };
	//
private:
	UISettings( QWidget* = 0 );
	void loadSettings();
	void initLexers();
	void saveSettings();
	QPixmap colourizedPixmap( const QColor& ) const;
	QColor iconBackgroundColor( const QIcon& ) const;
	static QPointer<UISettings> mSelf;
	QHash<QString,QsciLexer*> mLexers;
	QButtonGroup* bgDesigner;
	QButtonGroup* bgExternalChanges;
	QButtonGroup* bgAPISource;
	QButtonGroup* bgEOLMode;
	QButtonGroup* bgPythonIndentationWarning;
	//
public slots:
	void accept();
	//
private slots:
	void cbAPIsLanguages_beforeChanged( int );
	void on_cbAPIsLanguages_currentIndexChanged( int );
	void on_pbAPIsDelete_clicked();
	void on_pbAPIsAdd_clicked();
	void on_pbAPIsBrowse_clicked();
	void tbColours_clicked();
	void on_twHighlighterAssociations_itemClicked( QTreeWidgetItem*, int );
	void on_pbHighlighterAssociationAddChange_clicked();
	void on_pbHighlighterAssociationDelete_clicked();
	void on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( const QString& );
	void on_lwSyntaxHighlightingStyleElements_itemClicked( QListWidgetItem* );
	void on_pbSyntaxHighlightingForegroundColour_clicked();
	void on_pbSyntaxHighlightingBackgroundColour_clicked();
	void on_pbSyntaxHighlightingFont_clicked();
	void on_cbSyntaxHighlightingFillToEndOfLine_clicked( bool );
	void on_pbSyntaxHighlightingAllBackgroundColours_clicked();
	void on_pbSyntaxHighlightingAllFonts_clicked();
	void on_pbStyleLineNumbersFont_clicked();
	void on_pbStyleMonospacedFont_clicked();
	//
};
//
#endif // UISETTINGS_H
