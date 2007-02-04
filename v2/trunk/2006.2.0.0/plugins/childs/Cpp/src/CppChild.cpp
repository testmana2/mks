#include "CppChild.h"
#include "CppCornerWidget.h"
// qt include
#include <QVBoxLayout>
#include <QSplitter>
#include <QFileInfo>
#include <QIcon>
// handled extensions
QStringList CppChild::mHeaderExtensions = QStringList() << "h" << "hh" << "hpp" << "h++" << "hxx";
QStringList CppChild::mSourceExtensions = QStringList() << "c" << "cc" << "cpp" << "c++" << "cxx";
//
CppChild::CppChild()
	: mInit( false )
{
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );
	mSplitter = new QSplitter( Qt::Vertical, this );
	vl->addWidget( mSplitter );
	connect( this, SIGNAL( layoutModeChanged( AbstractChild::LayoutMode ) ), this, SLOT( layoutModeChanged( AbstractChild::LayoutMode ) ) );
	// header
	mHeader = new CppMonkeyEditor;
	mHeader->setWindowIcon( QIcon( ":/Icons/Icons/h.png" ) );
	connect( mHeader, SIGNAL( cursorPositionChanged( const QPoint& ) ), this, SIGNAL( cursorPositionChanged( const QPoint& ) ) );
	connect( mHeader, SIGNAL( modificationChanged( bool ) ), this, SIGNAL( modifiedChanged( bool ) ) );
	connect( mHeader, SIGNAL( undoAvailable( bool ) ), this, SIGNAL( undoAvailableChanged( bool ) ) );
	connect( mHeader, SIGNAL( redoAvailable( bool ) ), this, SIGNAL( redoAvailableChanged( bool ) ) );
	connect( mHeader, SIGNAL( pasteAvailable( bool ) ), this, SIGNAL( pasteAvailableChanged( bool ) ) );
	connect( mHeader, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( copyAvailableChanged( bool ) ) );
	connect( mHeader, SIGNAL( focused( bool ) ), this, SIGNAL( updateWorkspaceRequested() ) );
	mSplitter->addWidget( mHeader );
	// source
	mSource = new CppMonkeyEditor;
	mSource->setWindowIcon( QIcon( ":/Icons/Icons/cpp.png" ) );
	connect( mSource, SIGNAL( cursorPositionChanged( const QPoint& ) ), this, SIGNAL( cursorPositionChanged( const QPoint& ) ) );
	connect( mSource, SIGNAL( modificationChanged( bool ) ), this, SIGNAL( modifiedChanged( bool ) ) );
	connect( mSource, SIGNAL( undoAvailable( bool ) ), this, SIGNAL( undoAvailableChanged( bool ) ) );
	connect( mSource, SIGNAL( redoAvailable( bool ) ), this, SIGNAL( redoAvailableChanged( bool ) ) );
	connect( mSource, SIGNAL( pasteAvailable( bool ) ), this, SIGNAL( pasteAvailableChanged( bool ) ) );
	connect( mSource, SIGNAL( copyAvailable( bool ) ), this, SIGNAL( copyAvailableChanged( bool ) ) );
	connect( mSource, SIGNAL( focused( bool ) ), this, SIGNAL( updateWorkspaceRequested() ) );
	mSplitter->addWidget( mSource );
}
//
void CppChild::showEvent( QShowEvent* e )
{
	AbstractChild::showEvent( e );
	if ( !mInit )
	{
		mInit = true;
		setLayoutMode( AbstractChild::lNormal );
	}
}
// give current editor
CppMonkeyEditor* CppChild::currentEditor() const
{
	if ( mHeader->hasFocus() )
		return mHeader;
	else if ( mSource->hasFocus() )
		return mSource;
	else
		return mHeader;
}
// return headers extension
QStringList CppChild::headerExtensions()
{
	return mHeaderExtensions;
}
// return source extensions
QStringList CppChild::sourceExtensions()
{
	return mSourceExtensions;
}
// left corner widget for this child
QWidget* CppChild::cornerWidget()
{
	CppCornerWidget::self()->setChild( this );
	return CppCornerWidget::self();
}
// return cursor position if available
QPoint CppChild::cursorPosition() const
{
	int l = 0, i = 0;
	currentEditor()->getCursorPosition( &l, &i );
	return QPoint( i +1, l +1 );
}
// show the current file in child
void CppChild::showFile( const QString& s )
{
	if ( mHeader->filePath() == s )
		mHeader->setFocus();
	else if ( mSource->filePath() == s )
		mSource->setFocus();
	layoutModeChanged( layoutMode() );
}
// the current visible / focused file
QString CppChild::currentFile() const
{
	return currentEditor()->filePath();
}
// return the current file modified flag
bool CppChild::isModified() const
{
	return currentEditor()->isModified();
}
// return the current file undo flag
bool CppChild::isUndoAvailable() const
{
	return currentEditor()->isUndoAvailable();
}
// return the current file redo flag
bool CppChild::isRedoAvailable() const
{
	return currentEditor()->isRedoAvailable();
}
// return the current file paste available
bool CppChild::isPasteAvailable() const
{
	return currentEditor()->pasteAvailable();
}
// return the current file copy available
bool CppChild::isCopyAvailable() const
{
	return currentEditor()->copyAvailable();
}
// return the modified state of file
bool CppChild::isModified( const QString& s ) const
{
	if ( mHeader->filePath() == s )
		return mHeader->isModified();
	else if ( mSource->filePath() == s )
		return mSource->isModified();
	return false;
}
// ask to save file
void CppChild::saveFile( const QString& s )
{
	if ( mHeader->filePath() == s )
		mHeader->saveFile();
	else if ( mSource->filePath() == s )
		mSource->saveFile();
}
//
void CppChild::saveFiles()
{
	mHeader->saveFile();
	mSource->saveFile();
}
// ask to load file
void CppChild::loadFile( const QString& s )
{
	QString e = QFileInfo( s ).completeSuffix();
	if ( mHeaderExtensions.contains( e, Qt::CaseInsensitive ) )
		mHeader->openFile( s );
	else if ( mSourceExtensions.contains( e, Qt::CaseInsensitive ) )
		mSource->openFile( s );
	showFile( s );
	setWindowTitle( fullPathBaseName() );
}
//
void CppChild::printFile( const QString& )
{
	currentEditor()->print();
}
// ask to quick print this file
void CppChild::quickPrintFile( const QString& )
{
	currentEditor()->print( true );
}
// undo
void CppChild::undo()
{
	currentEditor()->undo();
}
// redo
void CppChild::redo()
{
	currentEditor()->redo();
}
// cut
void CppChild::cut()
{
	currentEditor()->cut();
}
// copy
void CppChild::copy()
{
	currentEditor()->copy();
}
// paste
void CppChild::paste()
{
	currentEditor()->paste();
}
// search/replace
void CppChild::searchReplace()
{
	currentEditor()->searchReplace();
}
// go to
void CppChild::goTo()
{
	currentEditor()->goToLine();
}
// search/replace available
bool CppChild::isSearchReplaceAvailable() const
{
	return true;
}
// goto available
bool CppChild::isGoToAvailable() const
{
	return true;
}
// print available
bool CppChild::isPrintAvailable() const
{
	return true;
}
//
void CppChild::layoutModeChanged( AbstractChild::LayoutMode m )
{
	switch ( m )
	{
	case AbstractChild::lVertical:
		mSplitter->setOrientation( Qt::Vertical );
		if ( !mHeader->isVisible() )
			mHeader->show();
		if ( !mSource->isVisible() )
			mSource->show();
		break;
	case AbstractChild::lHorizontal:
		mSplitter->setOrientation( Qt::Horizontal );
		if ( !mHeader->isVisible() )
			mHeader->show();
		if ( !mSource->isVisible() )
			mSource->show();
		break;
	default:
		mSplitter->setOrientation( Qt::Vertical );
		if ( currentEditor() == mHeader )
		{
			if ( !mHeader->isVisible() )
				mHeader->show();
			if ( mSource->isVisible() )
				mSource->hide();
		}
		else
		{
			if ( mHeader->isVisible() )
				mHeader->hide();
			if ( !mSource->isVisible() )
				mSource->show();
		}
		break;
	}
	setWindowIcon( currentEditor()->windowIcon() );
}
