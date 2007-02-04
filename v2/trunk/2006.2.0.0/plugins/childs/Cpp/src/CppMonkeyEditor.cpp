#include "CppMonkeyEditor.h"
//
#include <QApplication>
// qscintilla include
#include <qscilexercpp.h>
#include <qsciapis.h>
//
QsciLexerCPP* CppMonkeyEditor::mLexer = 0L;
QsciAPIs* CppMonkeyEditor::mAPI = 0L;
//
CppMonkeyEditor::CppMonkeyEditor( QWidget* p )
	: MonkeyEditor( p )
{
	if ( !mLexer )
	{
		mLexer = new QsciLexerCPP( qApp );
		mAPI = new QsciAPIs( mLexer );
		if ( mAPI->isPrepared( "Qt4.api" ) )
			mAPI->loadPrepared( "Qt4.api" );
		mLexer->setAutoIndentStyle( AiMaintain );
	}
	//
	setLexer( mLexer );
	mLexer->setAPIs( mAPI );
	//
	setFolding( BoxedTreeFoldStyle );
	setBraceMatching( SloppyBraceMatch );
	setAutoCompletionSource( AcsAll );
	setAutoCompletionThreshold( 2 );
	setAutoIndent( true );
	setIndentationsUseTabs( true );
	setAutoCompletionShowSingle( true );
}
//
CppMonkeyEditor::~CppMonkeyEditor()
{
}
//
bool CppMonkeyEditor::openFile( const QString& s, QTextCodec* c )
{
	bool b = MonkeyEditor::openFile( s, c );
	if ( b )
	{
		mAPI->load( s );
		mAPI->prepare();
	}
	return b;
}
