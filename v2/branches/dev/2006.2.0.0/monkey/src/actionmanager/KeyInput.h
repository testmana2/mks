#ifndef _KEY_INPUT_H
#define _KEY_INPUT_H
#include <QLineEdit>

class KeySequenceInput : public QLineEdit
{
private:
	bool controlPressed;
	bool altPressed;
	bool winPressed;
	bool shiftPressed;

public:
	KeySequenceInput ();
	
protected:
	//maybe possible just overload QObject::event (), but there is some problems	
	bool eventFilter(QObject *obj, QEvent *event);
};
#endif //_KEY_INPUT_H
