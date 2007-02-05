 #ifndef CPPMONKEYEDITOR_H
#define CPPMONKEYEDITOR_H
//
#include "MonkeyEditor.h"
//
class CppMonkeyEditor : public MonkeyEditor
{
    Q_OBJECT
    //
public:
	CppMonkeyEditor( QWidget* = 0 );
	~CppMonkeyEditor();
	//
protected:
	void keyPressEvent( QKeyEvent* );
	//
};
//
#endif // CPPMONKEYEDITOR_H
