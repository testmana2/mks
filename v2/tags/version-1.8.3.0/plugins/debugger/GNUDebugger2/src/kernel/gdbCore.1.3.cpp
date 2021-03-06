
#include "gdbCore.1.3.h"


GdbCore::GdbCore(QObject * parent) : QObject (parent)
{
	mParser = GdbParser::instance(); 
	mProcess = GdbProcess::instance(); 
	watchDog.setSingleShot(true);
	connect(&watchDog, SIGNAL(timeout()), this , SLOT(onTimer()));
	
	setWantAllMessages(false);
	setEnabled(true);
	setWaitEndProcess(false);
}

GdbCore::~GdbCore()
{
}


void GdbCore::showMessage(QString s, int t, SHOW c)
{
	QColor b;
	switch(c)
	{
	case _WARNING_ : b.setRgb(240,240,100); break;
	case _INFO_ : b.setRgb(120,250,100); break;
	case _CRITICAL_ : b.setRgb(255,35,35);

	}

	MonkeyCore::statusBar()->appendMessage( s, t ,QPixmap(), QBrush(b));
}


void GdbCore::interpreter(const QPointer<BaseInterpreter> & , const int & , const QString & ){}

void GdbCore::setEnabled(const bool & b ) { mEnabled = b;}
bool GdbCore::isEnabled() { return mEnabled;}

void GdbCore::setWaitEndProcess(const bool & p){ if(p ) watchDog.start(5000); else watchDog.stop();/* qDebug() << "set wait " + QString::number(p) + " " +  name(); */ mWaitEndProcess = p;}

bool GdbCore::isWaitEndProcess() { /*qDebug() << "get wait " + QString::number(mWaitEndProcess) + " " +  name(); */return mWaitEndProcess;}

void GdbCore::onTimer()
{
	if(isWaitEndProcess())
	{
		showMessage("Detecting kernel panic : " + name() , 2500, _WARNING_);
		setWaitEndProcess(false);
	}
}

QString GdbCore::findValue(const QString & st , const QString & key)
{
	QStringList l = st.split("\",");
	
	foreach(QString s, l)
	{
		QString k = s.left(s.indexOf("="));
		QString v = s.right(s.length() - s.indexOf("=") - 1);
		if(k == key ) return v.remove("\"");
	}
	return QString();
}
