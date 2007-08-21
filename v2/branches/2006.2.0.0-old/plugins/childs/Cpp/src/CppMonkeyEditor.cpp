#include "CppMonkeyEditor.h"
//
#include <QApplication>
#include <QKeyEvent>
//
CppMonkeyEditor::CppMonkeyEditor( QWidget* p )
	: MonkeyEditor( p )
{
	setFolding( BoxedTreeFoldStyle );
	setBraceMatching( SloppyBraceMatch );
	setAutoCompletionSource( AcsAll );
	setAutoIndent( true );
	setIndentationsUseTabs( true );
}
//
CppMonkeyEditor::~CppMonkeyEditor()
{
}
//
void CppMonkeyEditor::keyPressEvent( QKeyEvent* e )
{
	if ( !e->isAutoRepeat() && !isListActive() && e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_Space )
	{
		switch ( autoCompletionSource() )
		{
		case AcsAll:
			autoCompleteFromAll();
			break;
		case AcsDocument:
			autoCompleteFromDocument();
			break;
		case AcsAPIs:
			autoCompleteFromAPIs();
			break;
		case AcsNone:
		default:
			MonkeyEditor::keyPressEvent( e );
			break;
		}
	}
	else
		MonkeyEditor::keyPressEvent( e );
}
