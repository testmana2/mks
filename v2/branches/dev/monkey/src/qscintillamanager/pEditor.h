#ifndef PEDITOR_H
#define PEDITOR_H

#include "qsciscintilla.h"

#include "MonkeyExport.h"

class Q_MONKEY_EXPORT pEditor : public QsciScintilla
{
    Q_OBJECT

public:
	pEditor( QWidget* = 0 );
	virtual ~pEditor();

	bool canPaste();

protected slots:
	void cursorPositionChanged( int, int );
	void textChanged();
	void clipboardDataChanged();

public slots:
	bool openFile( const QString& );
	bool saveFile( const QString& );
	void closeFile();

signals:
	void cursorPositionChanged( const QPoint& );
	void undoAvailable( bool );
	void redoAvailable( bool );
	void pasteAvailable( bool );

};

#endif // PEDITOR_H
