#include "assistantSearch.h"
//
#include <QTextCursor>
//
assistantSearch::assistantSearch( QTextBrowser* p )
	: QWidget(), tbSource( p )
{
	setupUi( this );
}
//
void assistantSearch::on_tbPrevious_clicked()
{
	search( QTextDocument::FindBackward );
}
//
void assistantSearch::on_tbNext_clicked()
{
	search( 0 );
}
//
void assistantSearch::search( QTextDocument::FindFlags f )
{
	if ( cbCaseSensitive->isChecked() )
		f |= QTextDocument::FindCaseSensitively;
	if ( cbWholeWords->isChecked() )
		f |= QTextDocument::FindWholeWords;
	QTextCursor c = tbSource->document()->find( leSearch->text(), tbSource->textCursor(), f );
	if ( !c.isNull() )
		tbSource->setTextCursor( c );
	QPalette p = leSearch->palette();
	p.setColor( leSearch->backgroundRole(), c.isNull() ? Qt::red : Qt::white );
	leSearch->setPalette( p );
}
