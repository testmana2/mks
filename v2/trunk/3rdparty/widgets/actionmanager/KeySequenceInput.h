#ifndef _KEY_SEQUENCE_INPUT
#define _KEY_SEQUENCE_INPUT
#include <QLineEdit>

#include <QWidget>
#include <QKeyEvent>
#include <QLineEdit>

class KeySequenceInput : public QLineEdit
{
private:
	bool controlPressed;
	bool altPressed;
	bool winPressed;
	bool shiftPressed;

public:
	KeySequenceInput (QWidget*);
	
protected:
	//maybe possible just overload QObject::event (), but there is some problems	
	bool eventFilter(QObject *obj, QEvent *event);
};
#endif //_KEY_SEQUENCE_INPUT
