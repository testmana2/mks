#include "KeyInput.h"


KeySequenceInput::KeySequenceInput ()
{
	controlPressed = false;
	altPressed = false;
	winPressed = false;
	shiftPressed = false;
	installEventFilter (this);
}


bool KeySequenceInput::eventFilter(QObject *obj, QEvent *event)
{
	qDebug ("adfasdfasdf");
	if (event->type() == QEvent::KeyPress) 
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		switch ( keyEvent->key())
		{
		case Qt::Key_Control :
			controlPressed = true;
			break;
		case Qt::Key_Alt:
			altPressed = true;
			break;
		case Qt::Key_Shift:
			shiftPressed = true;
			break;
		case Qt::Key_Super_R:
			winPressed = true;
			break;
		case Qt::Key_Super_L:
			winPressed = true;
			break;
		default:
			QString text;
			if ( shiftPressed )
				text += "Shift+";
			if ( controlPressed )
				text += "Ctrl+";
			if ( altPressed )
				text +="Alt+";
			if ( winPressed )
				text +="Win+";
			text += QKeySequence (keyEvent->key()).toString(QKeySequence::PortableText);
			setText (text);
		} 
		return true;
	}
	if(event->type() == QEvent::KeyRelease)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		switch ( keyEvent->key())
		{
		case Qt::Key_Control :
			controlPressed = false;
			break;
		case Qt::Key_Alt:
			altPressed = false;
			break;
		case Qt::Key_Shift:
			shiftPressed = false;
			break;
		case Qt::Key_Super_R:
			winPressed = false;
			break;
		case Qt::Key_Super_L:
			winPressed = false;
			break;
		} 
		return true;
	}
	// standard event processing
	return QObject::eventFilter(obj, event);
}