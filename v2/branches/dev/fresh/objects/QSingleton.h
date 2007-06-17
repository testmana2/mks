/*
	Autors :
	pasnox, pasnox@gmail.com
	klorydryk, benoit.piedallu@free.fr
	lud42fr, IrmatDen

	How to use :

	// single sur un QPushButton
	class MyPushButon : public QPushButton, public QSingleton<MyPushButon>
	{
  		friend class QSingleton<MyPushButon>;

	protected:
		MyPushButon( QWidget* p )
		: QPushButton( p ), QSingleton<MyPushButon>()
		{};
	};
	// in cpp
	MyPushButton* pb = MyPushButton::instance( parentWidget ); // initialise object
	// then u can retreive it with MyPushButton::instance() or using pb pointer
*/
#ifndef QSINGLETON_H
#define QSINGLETON_H

#include "MonkeyExport.h"

#include <QApplication>
#include <QWidget>
#include <QPointer>

template <class T>
class Q_MONKEY_EXPORT QSingleton
{
protected:
	QSingleton() {};
	virtual ~QSingleton() {};

public:
	template <typename P>
	static T* instance( P* );
	static T* instance();

private:
	static QPointer<T> mInstance;

};

template <class T>
QPointer<T> QSingleton<T>::mInstance = 0L;

template <class T>
template <typename P>
T* QSingleton<T>::instance( P* p )
{
	if ( !mInstance )
		mInstance = new T( p );
	return mInstance;
}

template <class T>
T* QSingleton<T>::instance()
{
	if ( !mInstance )
		mInstance = new T;
	return mInstance;
}

#endif // QSINGLETON_H
