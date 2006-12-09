#ifndef STATUSBAR_H
#define STATUSBAR_H
//
#include <QStatusBar>
#include <QPointer>
//
#include "AbstractChild.h"
#include "MonkeyExport.h"
//
class QLabel;
//
class Q_MONKEY_EXPORT StatusBar : public QStatusBar
{
	Q_OBJECT
	//
public:
	enum Type { tStatusTip = 0, tCursor, tState, tMode, tFilePath };
	//
	static StatusBar* self( QWidget* = 0 );
	//
	QLabel* label( Type );
	//
public slots:
	void setText( Type, const QString&, int i = 0 );
	void setPixmap( Type, const QPixmap&, int i = 0 );
	void setCursorPosition( const QPoint& );
	void setModified( bool );
	void setMode( AbstractChild::Mode );
	void setFileName( const QString& );
	//
private:
	StatusBar( QWidget* = 0 );
	void initialize();
	//
	static QPointer<StatusBar> mSelf;
	QLabel* mLabels[ 5 ];
	//
};
//
#endif // STATUSBAR_H
