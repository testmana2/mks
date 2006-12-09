/********************************************************************************************************
 * PROGRAM      : monkey
 * DATE - TIME  : lundi 30 janvier 2006 - 17h48
 * AUTHOR       : Nox P@sNox ( pasnox@hotmail.com )
 * FILENAME     : Settings.cpp
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#include "Settings.h"
#include "main.h"
//
#include <QApplication>
//
QPointer<Settings> Settings::mSelf = 0L;
//
Settings* Settings::current()
{
	if ( !mSelf )
		mSelf = new Settings();
	return mSelf;
}
//
Settings::Settings()
	: pSettings( PROGRAM_NAME, PROGRAM_VERSION )
{}
//
QStringList Settings::languages() const
{
	return QStringList()
	<< "C"
	<< "Abkhazian"
	<< "Afan"
	<< "Afar"
	<< "Afrikaans"
	<< "Albanian"
	<< "Amharic"
	<< "Arabic"
	<< "Armenian"
	<< "Assamese"
	<< "Aymara"
	<< "Azerbaijani"
	<< "Bashkir"
	<< "Basque"
	<< "Bengali"
	<< "Bhutani"
	<< "Bihari"
	<< "Bislama"
	<< "Breton"
	<< "Bulgarian"
	<< "Burmese"
	<< "Byelorussian"
	<< "Cambodian"
	<< "Catalan"
	<< "Chinese"
	<< "Corsican"
	<< "Croatian"
	<< "Czech"
	<< "Danish"
	<< "Dutch"
	<< "English"
	<< "Esperanto"
	<< "Estonian"
	<< "Faroese"
	<< "FijiLanguage"
	<< "Finnish"
	<< "French"
	<< "Frisian"
	<< "Gaelic"
	<< "Galician"
	<< "Georgian"
	<< "German"
	<< "Greek"
	<< "Greenlandic"
	<< "Guarani"
	<< "Gujarati"
	<< "Hausa"
	<< "Hebrew"
	<< "Hindi"
	<< "Hungarian"
	<< "Icelandic"
	<< "Indonesian"
	<< "Interlingua"
	<< "Interlingue"
	<< "Inuktitut"
	<< "Inupiak"
	<< "Irish"
	<< "Italian"
	<< "Japanese"
	<< "Javanese"
	<< "Kannada"
	<< "Kashmiri"
	<< "Kazakh"
	<< "Kinyarwanda"
	<< "Kirghiz"
	<< "Korean"
	<< "Kurdish"
	<< "Kurundi"
	<< "Laothian"
	<< "Latin"
	<< "Latvian"
	<< "Lingala"
	<< "Lithuanian"
	<< "Macedonian"
	<< "Malagasy"
	<< "Malay"
	<< "Malayalam"
	<< "Maltese"
	<< "Maori"
	<< "Marathi"
	<< "Moldavian"
	<< "Mongolian"
	<< "NauruLanguage"
	<< "Nepali"
	<< "Norwegian"
	<< "Nynorsk"
	<< "Occitan"
	<< "Oriya"
	<< "Pashto"
	<< "Persian"
	<< "Polish"
	<< "Portuguese"
	<< "Punjabi"
	<< "Quechua"
	<< "RhaetoRomance"
	<< "Romanian"
	<< "Russian"
	<< "Samoan"
	<< "Sangho"
	<< "Sanskrit"
	<< "Serbian"
	<< "SerboCroatian"
	<< "Sesotho"
	<< "Setswana"
	<< "Shona"
	<< "Sindhi"
	<< "Singhalese"
	<< "Siswati"
	<< "Slovak"
	<< "Slovenian"
	<< "Somali"
	<< "Spanish"
	<< "Sundanese"
	<< "Swahili"
	<< "Swedish"
	<< "Tagalog"
	<< "Tajik"
	<< "Tamil"
	<< "Tatar"
	<< "Telugu"
	<< "Thai"
	<< "Tibetan"
	<< "Tigrinya"
	<< "TongaLanguage"
	<< "Tsonga"
	<< "Turkish"
	<< "Turkmen"
	<< "Twi"
	<< "Uigur"
	<< "Ukrainian"
	<< "Urdu"
	<< "Uzbek"
	<< "Vietnamese"
	<< "Volapuk"
	<< "Welsh"
	<< "Wolof"
	<< "Xhosa"
	<< "Yiddish"
	<< "Yoruba"
	<< "Zhuang"
	<< "Zulu";
}
//
void Settings::setDefaultSettings()
{
/*
	QFont mFont = qApp->font();
#ifdef Q_OS_WIN32
	mFont.setFamily( "Lucida Console" );
#else
	mFont.setFamily( "Monospace" );
#endif
	// setting defaults options
	beginGroup( "Settings" );
		// First Time Setting
		setValue( "FirstTimeSetting", value( "FirstTimeSetting", false ).toBool() );
		beginGroup( "General" );
			setValue( "ReloadLastProject", true );
			setValue( "ReloadLastProjectFiles", true );
			setValue( "SaveBeforeBuilding", true );
			setValue( "ShowHeaderInformations", true );
			setValue( "CreateBackup", false );
			//mSettings->setValue( "RecentFiles", QStringList() );
			//mSettings->setValue( "RecentProjects", QStringList() );
		endGroup();
		beginGroup( "Scintilla" );
			beginGroup( "User" );
				setValue( "Enable", true );
				beginGroup( "Completion" );
					setValue( "Threshold", -1 );
					setValue( "StartCharacters", QString() );
					setValue( "Fillups", false );
					setValue( "FillupsCharacters", QString() );
					setValue( "CaseSensitivity", true );
					setValue( "ReplaceWord", false );
					setValue( "ShowSingle", false );
					setValue( "Mode", 2 );
					beginGroup( "Calltips" );
						setValue( "Background", QColor( Qt::white ).name() );
						setValue( "ForeGround", QColor( Qt::lightGray ).name() );
						setValue( "Highlight", QColor( Qt::darkBlue ).name() );
						setValue( "Visible", -1 );
					endGroup();
				endGroup();
				beginGroup( "Indentation" );
					setValue( "Enabled", true );
					setValue( "BackspaceUnindents", false );
					setValue( "UseTabs", true );
					setValue( "Width", 0 );
					beginGroup( "BraceMatching" );
						setValue( "Mode", 2 );
						setValue( "MatchedBackground", QColor( Qt::white ).name() );
						setValue( "MatchedForeground", QColor( Qt::red ).name() );
						setValue( "UnmatchedBackground", QColor( Qt::white ).name() );
						setValue( "UnmatchedForeground", QColor( Qt::blue ).name() );
					endGroup();
					beginGroup( "Guides" );
						setValue( "Enabled", false );
						setValue( "Background", QColor( Qt::darkGreen ).name() );
						setValue( "Foreground", QColor( Qt::green ).name() );
					endGroup();
					beginGroup( "Tab" );
						setValue( "Indents", false );
						setValue( "Width", 4 );
					endGroup();
				endGroup();
				beginGroup( "Editor" );
					setValue( "TextColor", QColor( Qt::black ).name() );
					setValue( "TextFont", mFont.toString() );
					setValue( "PaperColor", QColor( Qt::white ).name() );
					setValue( "UseUtf8", false );
					setValue( "WhitespaceVisibility", 0 );
					setValue( "WrapMode", 0 );
					beginGroup( "Caret" );
						setValue( "Foreground", QColor( Qt::black ).name() );
						setValue( "LineBackground", QColor( "steelblue" ).name() );
						setValue( "LineVisible", true );
						setValue( "Width", 8 );
					endGroup();
					beginGroup( "EndofLine" );
						setValue( "Mode", -1 );
						setValue( "Visible", false );
					endGroup();
					beginGroup( "Edge" );
						setValue( "Mode", 1 );
						setValue( "Color", QColor( Qt::red ).name() );
						setValue( "Column", 80 );
					endGroup();
					beginGroup( "Selection" );
						setValue( "Background", QColor( Qt::cyan ).name() );
						setValue( "Foreground", QColor( Qt::darkCyan ).name() );
					endGroup();
				endGroup();
				beginGroup( "Margin" );
					setValue( "1/LineNumbers", true );
					setValue( "1/MarkerMask", 0 );
					setValue( "1/Sensitivity", true );
					setValue( "1/WidthPixel", -1 );
					setValue( "1/WidthString", "0000" );
					setValue( "size", 1 );
					beginGroup( "MarkerColors" );
						setValue( "1/Background", QColor( Qt::white ).name() );
						setValue( "1/Foreground", QColor( Qt::black ).name() );
						setValue( "size", 1 );
					endGroup();
					beginGroup( "Margins" );
						setValue( "Background", QColor( Qt::gray ).name() );
						setValue( "Foreground", QColor( Qt::black ).name() );
						setValue( "Font", mFont.toString() );
					endGroup();
					beginGroup( "Folding" );
						setValue( "Mode", 1 );
						setValue( "Background", QColor( Qt::gray ).name() );
						setValue( "Foreground", QColor( Qt::black ).name() );
					endGroup();
				endGroup();
			endGroup();
		endGroup();
	endGroup();
*/
}
