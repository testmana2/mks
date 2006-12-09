#ifndef MONKEYEDITOR_H
#define MONKEYEDITOR_H
//
#include <qsciscintilla.h>
//
#include <QHash>
//
struct Action
{
	enum StateType { None = 0, TextLength, UndoAvailable, RedoAvailable,
					CopyAvailable, PasteAvailable, NotCopyAvailableAndTextLength,
					Enabled, Disabled };
	Action( const QString& caption, QObject* receiver = 0, const char* slot = 0,
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
class MonkeyEditor : public QsciScintilla
{
    Q_OBJECT
    //
public:
	MonkeyEditor( QWidget* = 0 );
	//
	bool pasteAvailable() const;
	bool getState( Action::StateType );
	//
protected:
	void contextMenuEvent( QContextMenuEvent* );
	//
private:
	bool mCopyAvailable;
	QList<Action> mActions;
	//
private slots:
	void setCopyAvailable( bool );
	void selectNone();
	void goToLine();
	//
};
//
#endif // MONKEYEDITOR_H
