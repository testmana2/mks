#include "qscintillaSearch.h"
#include "main.h"
//
#include <qsciscintilla.h>
#include <QMessageBox>
//
QPointer<qscintillaSearch> qscintillaSearch::mSelf = 0L;
//
qscintillaSearch* qscintillaSearch::self( QsciScintilla* p )
{
	if ( !mSelf )
		mSelf = new qscintillaSearch( p );
	return mSelf;
}
//
qscintillaSearch::qscintillaSearch( QsciScintilla* p )
	: QDockWidget( p )
{
	setupUi( this );
	setEditor( p );
}
//
bool qscintillaSearch::checkEditor()
{
	if ( !mEditor )
		setEnabled( false );
	return mEditor;
}
//
QsciScintilla* qscintillaSearch::editor() const
{
	return mEditor;
}
//
void qscintillaSearch::setEditor( QsciScintilla* e )
{
	mEditor = e;
	setEnabled( e );
};
//
bool qscintillaSearch::on_tbPrevious_clicked()
{
	if ( !checkEditor() )
		return false;
	//
	int x, y;
	mEditor->getCursorPosition( &y, &x );
	if ( cbFromStart->isChecked() )
	{
		x = 0;
		y = 0;
	}
	bool b = mEditor->findFirst( leSearch->text(), cbRegExp->isChecked(), cbCaseSensitive->isChecked(), cbWholeWords->isChecked(), cbWrap->isChecked(), false, y, x -mEditor->selectedText().length() );
	if ( cbFromStart->isChecked() )
		cbFromStart->setChecked( false );
	QPalette p = leSearch->palette();
	p.setColor( leSearch->backgroundRole(), b ? Qt::white : Qt::red );
	leSearch->setPalette( p );
	return b;
}
//
bool qscintillaSearch::on_tbNext_clicked()
{
	if ( !checkEditor() )
		return false;
	//
	int x, y;
	mEditor->getCursorPosition( &y, &x );
	if ( cbFromStart->isChecked() )
	{
		x = 0;
		y = 0;
	}
	bool b = mEditor->findFirst( leSearch->text(), cbRegExp->isChecked(), cbCaseSensitive->isChecked(), cbWholeWords->isChecked(), cbWrap->isChecked(), true, y, x );
	if ( cbFromStart->isChecked() )
		cbFromStart->setChecked( false );
	QPalette p = leSearch->palette();
	p.setColor( leSearch->backgroundRole(), b ? Qt::white : Qt::red );
	leSearch->setPalette( p );
	return b;
}
//
bool qscintillaSearch::on_tbReplace_clicked()
{
	if ( !checkEditor() )
		return false;
	//
	if ( leReplace->text().isEmpty() )
		return false;
	if ( mEditor->selectedText().isEmpty()	&& !on_tbNext_clicked() )
		return false;
	//
	QString mSelection = mEditor->selectedText();
	QString mSearch = leSearch->text();
	bool b = false;
	//
	if ( !cbRegExp->isChecked() )
	{
		if ( cbCaseSensitive->isChecked() && mSelection == mSearch )
		{
			mEditor->replace( leReplace->text() );
			b = true;
			on_tbNext_clicked();
		}
		else if ( !cbCaseSensitive->isChecked() && mSelection.toLower() == mSearch.toLower() )
		{
			mEditor->replace( leReplace->text() );
			b = true;
			on_tbNext_clicked();
		}
	}
	else
	{
		if ( QRegExp( mSearch, cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive ).exactMatch( mSelection ) )
		{
			mEditor->replace( leReplace->text() );
			b = true;
			on_tbNext_clicked();
		}
	}
	return b;
}
//
void qscintillaSearch::on_tbReplaceAll_clicked()
{
	if ( !checkEditor() )
		return;
	//
	int i = 0;
	while ( on_tbReplace_clicked() )
		i++;
	if ( i != 0 )
		QMessageBox::information( this, INFORMATION, tr( "%1 occurences replaced" ).arg( i ) );
}
