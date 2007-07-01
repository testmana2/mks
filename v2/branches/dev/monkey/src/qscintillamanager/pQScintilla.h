#ifndef PQSCINTILLA_H
#define PQSCINTILLA_H

#include "MonkeyExport.h"
#include "QSingleton.h"

#include "qsciscintilla.h"
#include "qscilexerbash.h"
#include "qscilexerbatch.h"
#include "qscilexercmake.h"
#include "qscilexercpp.h"
#include "qscilexercsharp.h"
#include "qscilexercss.h"
#include "qscilexerd.h"
#include "qscilexerdiff.h"
#include "qscilexerhtml.h"
#include "qscilexeridl.h"
#include "qscilexerjava.h"
#include "qscilexerjavascript.h"
#include "qscilexerlua.h"
#include "qscilexermakefile.h"
#include "qscilexerperl.h"
#include "qscilexerpov.h"
#include "qscilexerproperties.h"
#include "qscilexerpython.h"
#include "qscilexerruby.h"
#include "qscilexersql.h"
#include "qscilexertex.h"
#include "qscilexervhdl.h"

#include <QObject>
#include <QApplication>
#include <QHash>

class Q_MONKEY_EXPORT pQScintilla : public QObject, public QSingleton<pQScintilla>
{
	Q_OBJECT
	friend class QSingleton<pQScintilla>;

public:
	QHash<QString,QsciLexer*> lexers();
	QStringList languages() const;
	QsciLexer* lexer( const QString& );
	void readSettings();
	void writeSettings();
	bool setProperty( const QString&, QsciLexer*, const QVariant& );
	QVariant property( const QString&, QsciLexer* ) const;
	QHash<QString, QStringList> defaultSuffixes() const;
	QHash<QString, QStringList> suffixes() const;
	QsciLexer* lexerForFilename( const QString& );

private:
	pQScintilla( QObject* = QApplication::instance() );
	~pQScintilla();

	QHash<QString,QsciLexer*> mLexers;

};

#endif // PQSCINTILLA_H
