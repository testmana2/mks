#ifndef UISETTINGS_H
#define UISETTINGS_H

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "ui_UISettings.h"

class QsciLexer;

class Q_MONKEY_EXPORT UISettings : public QDialog, public Ui::UISettings, public QSingleton<UISettings>
{
	Q_OBJECT
	friend class QSingleton<UISettings>;

public:
	enum Designer { Embedded = 0, External };
	enum ExternalChanges { Nothing = 0, Alert, Reload };

private:
	UISettings( QWidget* = 0 );
	void loadSettings();
	void saveSettings();
	QPixmap colourizedPixmap( const QColor& ) const;
	QColor iconBackgroundColor( const QIcon& ) const;
	QButtonGroup* bgDesigner;
	QButtonGroup* bgExternalChanges;
	QButtonGroup* bgAPISource;
	QButtonGroup* bgCallTipsStyle;
	QButtonGroup* bgEOLMode;
	QButtonGroup* bgPythonIndentationWarning;

public slots:
	void accept();

private slots:
	void on_twMenu_itemSelectionChanged();
	void on_tbDefaultProjectsDirectory_clicked();
	void on_tbTemplatesPath_clicked();
	void on_pbAddTemplateType_clicked();
	void on_pbEditTemplateType_clicked();
	void on_pbRemoveTemplateType_clicked();
	void on_pbEditTemplate_clicked();
	void cbAPIsLanguages_beforeChanged( int );
	void on_cbAPIsLanguages_currentIndexChanged( int );
	void on_pbAPIsDelete_clicked();
	void on_pbAPIsAdd_clicked();
	void on_pbAPIsBrowse_clicked();
	void tbColours_clicked();
	void on_twHighlighterAssociations_itemSelectionChanged();
	void on_pbHighlighterAssociationAddChange_clicked();
	void on_pbHighlighterAssociationDelete_clicked();
	void on_cbSyntaxHighlightingLexerLanguage_currentIndexChanged( const QString& );
	void on_lwSyntaxHighlightingStyleElements_itemSelectionChanged();
	void on_pbSyntaxHighlightingForegroundColour_clicked();
	void on_pbSyntaxHighlightingBackgroundColour_clicked();
	void on_pbSyntaxHighlightingFont_clicked();
	void on_cbSyntaxHighlightingFillToEndOfLine_clicked( bool );
	void cbProperties_clicked( bool );
	void on_cbIndentationWarning_currentIndexChanged( int );
	void on_pbSyntaxHighlightingAllBackgroundColours_clicked();
	void on_pbSyntaxHighlightingAllFonts_clicked();
	void on_pbStyleLineNumbersFont_clicked();
	void on_pbStyleMonospacedFont_clicked();
	void on_twAbbreviations_itemSelectionChanged();
	void on_pbAddAbbreviation_clicked();
	void on_pbRemoveAbbreviation_clicked();
	void on_teAbbreviation_textChanged();

};

#endif // UISETTINGS_H
