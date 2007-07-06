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
	QButtonGroup* bgAutoCompletionSource;
	QButtonGroup* bgCallTipsStyle;
	QButtonGroup* bgBraceMatch;
	QButtonGroup* bgEdgeMode;
	QButtonGroup* bgFoldStyle;
	QButtonGroup* bgEolMode;
	QButtonGroup* bgWhitespaceVisibility;
	QButtonGroup* bgWrapMode;
	QButtonGroup* bgStartWrapVisualFlag;
	QButtonGroup* bgEndWrapVisualFlag;

public slots:
	void accept();
	void apply();

private slots:
	void on_twMenu_itemSelectionChanged();
	void on_tbDefaultProjectsDirectory_clicked();
	void on_tbTemplatesPath_clicked();
	void on_pbAddTemplateType_clicked();
	void on_pbEditTemplateType_clicked();
	void on_pbRemoveTemplateType_clicked();
	void on_pbEditTemplate_clicked();
	void tbColours_clicked();
	void tbFonts_clicked();
	void cbSourceAPIsLanguages_beforeChanged( int );
	void on_cbSourceAPIsLanguages_currentIndexChanged( int );
	void on_pbSourceAPIsDelete_clicked();
	void on_pbSourceAPIsAdd_clicked();
	void on_pbSourceAPIsBrowse_clicked();
	void on_twLexersAssociations_itemSelectionChanged();
	void on_pbLexersAssociationsAddChange_clicked();
	void on_pbLexersAssociationsDelete_clicked();
	void on_cbLexersHighlightingLanguages_currentIndexChanged( const QString& );
	void on_lwLexersHighlightingElements_itemSelectionChanged();
	void lexersHighlightingColour_clicked();
	void lexersHighlightingFont_clicked();
	void on_cbLexersHighlightingFillEol_clicked( bool );
	void cbLexersHighlightingProperties_clicked( bool );
	void on_cbLexersHighlightingIndentationWarning_currentIndexChanged( int );
	void on_pbLexersHighlightingReset_clicked();
	void on_twAbbreviations_itemSelectionChanged();
	void on_pbAbbreviationsAdd_clicked();
	void on_pbAbbreviationsRemove_clicked();
	void on_teAbbreviationsCode_textChanged();

};

#endif // UISETTINGS_H
