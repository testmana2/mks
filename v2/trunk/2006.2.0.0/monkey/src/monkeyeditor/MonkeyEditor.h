#ifndef MONKEYEDITOR_H
#define MONKEYEDITOR_H
//
#include "qsciscintilla.h"
//
#include "MonkeyExport.h"
#include "Settings.h"
//
#include <QHash>
//
struct Q_MONKEY_EXPORT pAction
{
	enum StateType { None = 0, TextLength, UndoAvailable, RedoAvailable,
					CopyAvailable, PasteAvailable, NotCopyAvailableAndTextLength,
					Enabled, Disabled };
	pAction( const QString& caption, QObject* receiver = 0, const char* slot = 0,
		StateType state = Enabled, const QString& shortcut = QString(), const QString& pixmap = QString() )
		: Caption( caption ), Receiver( receiver ), Slot( slot ), State( state ),
			Shortcut( shortcut ), Pixmap( pixmap ) {}
	//
	QString Caption;
	QObject* Receiver;
	const char* Slot;
	StateType State;
	QString Shortcut;
	QString Pixmap;
};
//
class Q_MONKEY_EXPORT MonkeyEditor : public QsciScintilla
{
    Q_OBJECT
    //
public:
	enum WriteMode { wmNone = 0, wmInsert, wmOverwrite };
	enum Bookmarks { bm0 = 0, 	bm1, bm2, bm3, bm4, bm5, bm6, bm7, bm8, bm9 };
	//
	struct Q_MONKEY_EXPORT pBookmark
	{
		QPixmap* Pixmap;
		int Index;
		int Handle;
	};
	//
	MonkeyEditor( QWidget* = 0 );
	virtual ~MonkeyEditor();
	//
	static MonkeyEditor* createEditorForFilename( const QString&, const QPoint& = QPoint(), QTextCodec* = Settings::defaultCodec(), QWidget* = 0 );
	//
	virtual bool pasteAvailable() const;
	virtual bool copyAvailable() const;
	virtual bool getState( pAction::StateType );
	//
	virtual void print( bool = false );
	virtual void quickPrint();
	//
	virtual QString filePath() const { return mFilePath; }
	//
protected:
	virtual void focusInEvent( QFocusEvent* );
	virtual void focusOutEvent( QFocusEvent* );
	virtual void contextMenuEvent( QContextMenuEvent* );
	virtual void keyPressEvent( QKeyEvent* );
	virtual void checkBookmarks( Bookmarks, int );
	//
private:
	bool mCopyAvailable;
	QList<pAction*> mActions;
	QTextCodec* mCodec;
	QString mFilePath;
	QHash<Bookmarks, pBookmark*> mBookmarks;
	//
private slots:
	virtual void clipboard_dataChanged();
	virtual void cursorPositionChanged( int, int );
	virtual void textChanged();
	virtual void setCopyAvailable( bool );
	virtual void marginClicked( int, int, Qt::KeyboardModifiers );
	//
public slots:
	virtual void selectNone();
	virtual void searchReplace();
	virtual void goToLine();
	virtual bool openFile( const QString&, const QPoint& = QPoint(), QTextCodec* = Settings::defaultCodec() );
	virtual void saveFile();
	virtual void closeFile();
	//
signals:
	void cursorPositionChanged( const QPoint& );
	void undoAvailable( bool );
	void redoAvailable( bool );
	void pasteAvailable( bool );
	void fileOpened( bool );
	void focused( bool );
	// debugger signal
	//
};
//
#endif // MONKEYEDITOR_H
