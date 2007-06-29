#ifndef PEDITOR_H
#define PEDITOR_H

#include "qsciscintilla.h"

#include "MonkeyExport.h"

class Q_MONKEY_EXPORT pEditor : public QsciScintilla
{
    Q_OBJECT

protected:
	bool mCopyAvailable;
	QPoint mCursorPosition;

public:
	pEditor( QWidget* = 0 );
	virtual ~pEditor();

	bool copyAvailable();
	bool canPaste();
	QPoint cursorPosition() const;

protected slots:
	void setCopyAvailable( bool );
	void cursorPositionChanged( int, int );
	void textChanged();
	void clipboardDataChanged();

public slots:
	bool openFile( const QString& );
	bool saveFile( const QString& );
	void closeFile();
	void print( bool = false );
	void quickPrint();
	void selectNone();
	void invokeSearchReplace();
	void invokeGoToLine();
	void convertTabs( int i );

signals:
	void cursorPositionChanged( const QPoint& );
	void undoAvailable( bool );
	void redoAvailable( bool );
	void pasteAvailable( bool );

};

#endif // PEDITOR_H
