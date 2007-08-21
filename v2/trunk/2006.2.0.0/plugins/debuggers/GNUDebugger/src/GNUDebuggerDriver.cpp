//======================================
// Autor : Xiantia
//
//CONTROLER GDB
//======================================


// INFO : pas de bug trouv


#include "GNUDebuggerDriver.h"
#include <QMessageBox>
#include <QTime>



// logger tout les message de gdb
#define LOG_GDB		true

#ifdef Q_OS_WIN32
#define NAME_FILE_LOG	"./log_gdb.txt"
#else
#define NAME_FILE_LOG	"./log_gdb.txt"
#endif

QTime mytime;


GNUDebuggerDriver::GNUDebuggerDriver(  )
{
	
	// fix by Xiantia
	GdbName = "gdb";
	#ifdef Q_OS_WIN32
		if ( !	GdbName.endsWith( ".exe", Qt::CaseInsensitive ) )
			GdbName.append( ".exe" );
	#endif

	gdbBusy=false;
	gdbStarted = false;

	mpGdbProcess = new QProcess(this);
	mpGdbProcess->setProcessChannelMode( QProcess::MergedChannels );
	connect(mpGdbProcess,SIGNAL(readyRead()),this, SLOT(onReadAll()));

	connect(this,SIGNAL(signalGdbCommandFinish()),this,SLOT(slotGdbCommandFinish()));

	connect(mpGdbProcess,SIGNAL(error(QProcess::ProcessError )),this,SLOT(OnError(QProcess::ProcessError )));

	connect(mpGdbProcess,SIGNAL(finished ( int , QProcess::ExitStatus  )),this,SLOT(gdbfinished ( int , QProcess::ExitStatus  )));

	file_log_gdb = NULL;

	openLogFile();

	mpGdbProcess->start(GdbName);
}


GNUDebuggerDriver::~GNUDebuggerDriver(  )
{
	closeLogFile();
	delete mpGdbProcess;
}



void GNUDebuggerDriver::gdbfinished ( int a, QProcess::ExitStatus  b)
{
	// no warning
	a=a;
	b=b;
	
	mpGdbProcess->close();
	mpGdbProcess-> terminate ();
	mpGdbProcess-> kill ();

//	qDebug("Gdb killed");
	delete this;
}



void GNUDebuggerDriver::setCommand(QString command)
{
	if(!command.isEmpty())
		 mCommandList << command + "\r\n";

	if(gdbBusy && gdbStarted)
	{
		mytime.restart();
		gdbBusy = false;
		emit signalGdbCommandFinish();
	}
}


void GNUDebuggerDriver::slotGdbCommandFinish()
{
	int numberCommand = mCommandList.count();
	if(numberCommand != 0)
	{
		gdbBusy = false;

		writeLogFile("start send command -> temps ecoule apres le signal " + QString::number(mytime.elapsed()) +"\r\n" + mCommandList.value(0).toAscii());

		mytime.restart();
		mpGdbProcess->write(mCommandList.value(0).toAscii());
		mCommandList.removeAt(0);
	}
}

void GNUDebuggerDriver::OnError(QProcess::ProcessError er)
{
	switch(er)
	{
		case QProcess::FailedToStart : 
			QMessageBox::critical(0,tr("Error"), tr("Can't start ") + GdbName + tr("\nMake sur you haved this in your path"), QMessageBox::Ok, QMessageBox::NoButton);
		break;
		default : QMessageBox::critical(0,tr("Error"), tr("Gdb have generate a unknow error"), QMessageBox::Ok, QMessageBox::NoButton);

	}
}


void GNUDebuggerDriver::onReadAll()
{
	st.append(QString::fromLocal8Bit( mpGdbProcess->readAll() ));

	/* 
	On start gdb
	*/

	if(st.contains("This GDB was configured as"))
	{
		writeLogFile("gdb Started OK\r\n");

		gdbStarted = true;
		gdbBusy = true;	
		emit gdbDataAvailable(st.remove("(gdb) "));
		emit signalGdbCommandFinish();
		st.clear();
		return;
	}


	if(st.contains/*endsWith*/("(gdb) "))// || st.contains("? (y or n)"))
	{
		writeLogFile("---> temps de reponce " + QString::number(mytime.elapsed()) + "\r\n" + st.toAscii() + "\r\n---<\r\n");

		emit gdbDataAvailable(st.remove("(gdb) "));
		st.clear();
		gdbBusy = true;

		emit signalGdbCommandFinish();
		return;
	}

return;
}


// log file
void GNUDebuggerDriver::openLogFile()
{
	if(LOG_GDB)
	{
		file_log_gdb = new QFile(NAME_FILE_LOG );
			if (!file_log_gdb->open(QIODevice::Append)) file_log_gdb = NULL;
	}

}

void GNUDebuggerDriver::writeLogFile(QString st)
{
	QTextStream out(file_log_gdb);

	if(LOG_GDB && file_log_gdb)
	{
		out << st;
		out.flush();
	}
}


void GNUDebuggerDriver::closeLogFile()
{
	if(LOG_GDB && file_log_gdb) file_log_gdb->close();
}
