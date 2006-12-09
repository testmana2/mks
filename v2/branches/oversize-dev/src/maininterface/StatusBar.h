#ifndef STATUSBAR_H
#define STATUSBAR_H
//
#include <QStatusBar>
#include <QPointer>
//
class QLabel;
//
class StatusBar : public QStatusBar
{
	Q_OBJECT
	//
public:
	enum LabelType { ltStatusTip = 0, ltCursor, ltState, ltMode, ltFilePath };
	enum LabelMode { lmNone = 0, lmInsert, lmOverwrite };
	//
	static StatusBar* self( QWidget* = 0 );
	//
	void initialize();
	QLabel* label( LabelType );
	//
public slots:
	void setText( LabelType, const QString&, int i = 0 );
	void setCursorPosition( const QPoint& );
	void setModified( bool );
	void setMode( LabelMode );
	void setFileName( const QString& );
	//
private:
	StatusBar( QWidget* = 0 );
	//
	static QPointer<StatusBar> mSelf;
	QLabel* mLabels[ 5 ];
	//
};
//
#endif // STATUSBAR_H
