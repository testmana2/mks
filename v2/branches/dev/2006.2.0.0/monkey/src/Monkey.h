#ifndef MONKEY_H
#define MONKEY_H
//
#include <QApplication>
#include <QPointer>
//
#include "MonkeyExport.h"
//
class Q_MONKEY_EXPORT Monkey : public QApplication
{
	Q_OBJECT
	//
public:
	static Monkey* self( int&, char** );
	//
	void initialize();
	//
private:
	Monkey( int&, char** );
	//
	static QPointer<Monkey> mSelf;
	//
};
//
#endif // MONKEY_H
