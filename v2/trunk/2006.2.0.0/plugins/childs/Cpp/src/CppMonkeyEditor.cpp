#include "CppMonkeyEditor.h"
//
#include <QApplication>
#include <QKeyEvent>
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
		qWarning( "trying to load qt4 api" );
		if ( mAPI->isPrepared( "Qt4_Prepared.api" ) )
		{
			qWarning( "api is prepared" );
			//mAPI->prepare();
			mAPI->loadPrepared( "Qt4_Prepared.api" );
		}
		qWarning( "api loaded" );
		mLexer->setAutoIndentStyle( AiMaintain );
	}
	//
	setLexer( mLexer );
	mLexer->setAPIs( mAPI );
	//
	setFolding( BoxedTreeFoldStyle );
	setBraceMatching( SloppyBraceMatch );
	setAutoCompletionSource( AcsAll );
	//setAutoCompletionThreshold( -1 );
	setAutoIndent( true );
	setIndentationsUseTabs( true );
	setAutoCompletionShowSingle( true );
	setAutoCompletionWordSeparators( QStringList() << "." << "->" );
}
//
CppMonkeyEditor::~CppMonkeyEditor()
{
	//mAPI->savePrepared( "Qt4_Prepared.api" );
}
//
void CppMonkeyEditor::keyReleaseEvent( QKeyEvent* e )
{
	e->ignore();
	return;
/*
	if ( !e->isAutoRepeat() && !isListActive() && e->matches( QKeySentence( tr( "Ctrl+Space" ) ) ) )
	{
		
		e->accept();
		return;
	}
	e->ignore();*/
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
