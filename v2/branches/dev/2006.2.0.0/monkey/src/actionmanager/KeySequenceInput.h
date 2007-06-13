#ifndef _KEY_SEQUENCE_INPUT
#define _KEY_SEQUENCE_INPUT

#include <QLineEdit>
#include <QKeyEvent>
/*
Class may be used for defining key sequences. It's lools as QLineEdit,
but when user pressing some key/combination of keys, 
widget will display it in the fit of key sequence
See ActionManager.h for more comments

Author - Kopats Andrei. 
hlamer at tut dot by
Writed for the Monkey Developer Studio project
(www.monkeystudio.org)
Date: 14/06/2007
License: GPL
*/

class KeySequenceInput : public QLineEdit
{

private:
	bool controlPressed;
	bool altPressed;
	bool winPressed;
	bool shiftPressed;
	
public:
	KeySequenceInput ( QWidget* );
	
protected:
	//maybe possible just overload QObject::event (), but there is some problems
	bool eventFilter( QObject *obj, QEvent *event );
};
#endif //_KEY_SEQUENCE_INPUT

