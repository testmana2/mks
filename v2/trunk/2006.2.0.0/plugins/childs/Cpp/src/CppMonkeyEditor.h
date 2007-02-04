 #ifndef CPPMONKEYEDITOR_H
#define CPPMONKEYEDITOR_H
//
#include "MonkeyEditor.h"
//
class QsciLexerCPP;
class QsciAPIs;
//
class CppMonkeyEditor : public MonkeyEditor
{
    Q_OBJECT
    //
public:
	CppMonkeyEditor( QWidget* = 0 );
	~CppMonkeyEditor();
	//
private:
	static QsciLexerCPP* mLexer;
	static QsciAPIs* mAPI;
	//
public slots:
	virtual bool openFile( const QString&, QTextCodec* = QTextCodec::codecForLocale() );
	//
};
//
#endif // CPPMONKEYEDITOR_H
