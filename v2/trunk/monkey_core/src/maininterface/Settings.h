#ifndef SETTINGS_H
#define SETTINGS_H
//
#include <QSettings>
//
#include "QSingleton.h"
#include "main.h"
//
class QMainWindow;
class QTextCodec;
class QsciLexer;
//
class Q_MONKEY_EXPORT Settings : public QSettings, public QSingleton<Settings>
{
	Q_OBJECT
	friend class QSingleton<Settings>;
	//
private:
	Settings( const QString& = PROGRAM_NAME, const QString& = PROGRAM_VERSION );
	~Settings();
	//
	QString mProgramName;
	QString mProgramVersion;
	//
public:
	QString programName() const;
	QString programVersion() const;
	//
	void restoreState( QMainWindow* );
	void saveState( QMainWindow* );
	//
	void setDefaultSettings();
	//
	static QTextCodec* defaultCodec();
	static QsciLexer* lexerForLanguage( const QString& );
	static QsciLexer* lexerForFilename( const QString& );
	static QString languageForFilename( const QString& );
	static QStringList lexersFilters();
	static QString dialogLexersFilters();
	//
};
//
#endif // SETTINGS_H
