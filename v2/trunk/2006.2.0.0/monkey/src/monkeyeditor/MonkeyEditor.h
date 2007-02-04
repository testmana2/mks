#ifndef MONKEYEDITOR_H
#define MONKEYEDITOR_H
//
#include <qsciscintilla.h>
//
#include <QHash>
#include <QTextCodec>
//
#include "MonkeyExport.h"
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
	//
	MonkeyEditor( QWidget* = 0 );
	virtual ~MonkeyEditor();
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
	//
private:
	bool mCopyAvailable;
	QList<pAction*> mActions;
	QTextCodec* mCodec;
	QString mFilePath;
	//
private slots:
	virtual void clipboard_dataChanged();
	virtual void cursorPositionChanged( int, int );
	virtual void textChanged();
	virtual void setCopyAvailable( bool );
	//
public slots:
	virtual void selectNone();
	virtual void searchReplace();
	virtual void goToLine();
	virtual bool openFile( const QString&, QTextCodec* = QTextCodec::codecForLocale() );
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
	//
};
//
#endif // MONKEYEDITOR_H
