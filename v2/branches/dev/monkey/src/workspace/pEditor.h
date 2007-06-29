#ifndef PEDITOR_H
#define PEDITOR_H

#include "qsciscintilla.h"

#include "MonkeyExport.h"

#include <QHash>

class Q_MONKEY_EXPORT pEditor : public QsciScintilla
{
    Q_OBJECT

public:
	enum WriteMode { wmNone = 0, wmInsert, wmOverwrite };
	enum Bookmarks { bm0 = 0, bm1, bm2, bm3, bm4, bm5, bm6, bm7, bm8, bm9 };

	struct Q_MONKEY_EXPORT pBookmark
	{
		QPixmap* Pixmap;
		int Index;
		int Handle;
	};

	pEditor( QWidget* = 0 );
	virtual ~pEditor();

	static pEditor* createEditorForFilename( const QString&, const QPoint& = QPoint(), QTextCodec* = Settings::defaultCodec(), QWidget* = 0 );

	virtual bool pasteAvailable() const;
	virtual bool copyAvailable() const;

	virtual void print( bool = false );
	virtual void quickPrint();

	virtual QString filePath() const { return mFilePath; }

protected:
	virtual void readSettings();
	virtual void focusInEvent( QFocusEvent* );
	virtual void focusOutEvent( QFocusEvent* );
	virtual void contextMenuEvent( QContextMenuEvent* );
	virtual void keyPressEvent( QKeyEvent* );
	virtual void checkBookmarks( Bookmarks, int );

private:
	bool mCopyAvailable;
	QTextCodec* mCodec;
	QString mFilePath;
	QHash<Bookmarks, pBookmark*> mBookmarks;

private slots:
	virtual void clipboard_dataChanged();
	virtual void cursorPositionChanged( int, int );
	virtual void textChanged();
	virtual void setCopyAvailable( bool );
	virtual void marginClicked( int, int, Qt::KeyboardModifiers );

public slots:
	virtual void selectNone();
	virtual void searchReplace();
	virtual void goToLine();
	virtual bool openFile( const QString&, const QPoint& = QPoint(), QTextCodec* = 0 );
	virtual void saveFile();
	virtual void closeFile();
	virtual void setAutoSaveInterval( int );
	virtual void convertTabs( int = -1 );

signals:
	void cursorPositionChanged( const QPoint& );
	void undoAvailable( bool );
	void redoAvailable( bool );
	void pasteAvailable( bool );
	void fileOpened( bool );
	void focused( bool );
	//
};

#endif // PEDITOR_H
