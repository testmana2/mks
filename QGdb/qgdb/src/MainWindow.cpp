#include <QLabel>
#include <QFileDialog>
#include <QDir>

#include "MainWindow.h"

MainWindow::MainWindow( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
	connect (actionLoad_target, SIGNAL (triggered()), this, SLOT (loadTargetTriggered()));
	
	connect (actionRun, SIGNAL (triggered()), this, SIGNAL (exec_runTriggered()));
	connect (actionContinue, SIGNAL (triggered()), this, SIGNAL (exec_continueTriggered()));
	connect (actionStep_into, SIGNAL (triggered()), this, SIGNAL (exec_stepIntoTriggered()));
	connect (actionStep_over, SIGNAL (triggered()), this, SIGNAL (exec_stepOverTriggered()));
	connect (actionStep_out, SIGNAL (triggered()), this, SIGNAL (exec_stepOutTriggered()));
	connect (actionPause, SIGNAL (triggered()), this, SIGNAL (exec_pauseTriggered()));
	connect (actionKill, SIGNAL (triggered()), this, SIGNAL (exec_killTriggered()));
	
	mStatusLabel = new QLabel();
	
	QMainWindow::statusBar()->addWidget (mStatusLabel);
}

MainWindow::~MainWindow()
{
}

void MainWindow::loadTargetTriggered()
{
	QString fileName = QFileDialog::getOpenFileName (this, tr("Load target"), QDir::current().path());
	if (! fileName.isEmpty())
		emit loadTargetRequested (fileName);
}

void MainWindow::appendLog( const QString& log )
{
	pteLog->appendPlainText( log );
}

void MainWindow::onDebuggerCallbackMessage( const QString& message, QGdbDriver::CBType type )
{
	switch ( type )
	{
		case QGdbDriver::CONSOLE:
			appendLog( QString( "CONSOLE> %1" ).arg( message ) );
			break;
		case QGdbDriver::TARGET:
			appendLog( QString( "TARGET> %1" ).arg( message ) );
			break;
		case QGdbDriver::LOG:
			appendLog( QString( "LOG> %1" ).arg( message ) );
			break;
		case QGdbDriver::TO_GDB:
			appendLog( QString( ">> %1" ).arg( message ) );
			break;
		case QGdbDriver::FROM_GDB:
			appendLog( QString( "<< %1" ).arg( message ) );
			break;
		case QGdbDriver::ASYNC:
			appendLog( QString( "ASYNC> %1" ).arg( message ) );
			break;
	}
}

void MainWindow::onDebuggerStateChanged( QGdbDriver::State state )
{
	switch (state)
	{
		case QGdbDriver::CONNECTED:
			actionRun->setEnabled (false);
			actionContinue->setEnabled (false);
			actionStep_into->setEnabled (false);
			actionStep_over->setEnabled (false);
			actionStep_out->setEnabled (false);
			actionPause->setEnabled (false);
			actionKill->setEnabled (false);
			mStatusLabel->setText (tr("Connected to GDB"));
		break;
		case QGdbDriver::DISCONNECTED:
			actionRun->setEnabled (false);
			actionContinue->setEnabled (false);
			actionStep_into->setEnabled (false);
			actionStep_over->setEnabled (false);
			actionStep_out->setEnabled (false);
			actionPause->setEnabled (false);
			actionKill->setEnabled (false);
			mStatusLabel->setText (tr("Disconnected from GDB"));
		break;
		case QGdbDriver::TARGET_SETTED:
			actionRun->setEnabled (true);
			actionContinue->setEnabled (false);
			actionStep_into->setEnabled (false);
			actionStep_over->setEnabled (false);
			actionStep_out->setEnabled (false);
			actionPause->setEnabled (false);
			actionKill->setEnabled (false);
			mStatusLabel->setText (tr("Target loaded"));
		break;
		case QGdbDriver::RUNNING:
			actionRun->setEnabled (true);
			actionContinue->setEnabled (false);
			actionStep_into->setEnabled (false);
			actionStep_over->setEnabled (false);
			actionStep_out->setEnabled (false);
			actionPause->setEnabled (true);
			actionKill->setEnabled (true);
			mStatusLabel->setText (tr("Target is running"));
		break;
		case QGdbDriver::STOPPED:
			actionRun->setEnabled (true);
			actionContinue->setEnabled (true);
			actionStep_into->setEnabled (true);
			actionStep_over->setEnabled (true);
			actionStep_out->setEnabled (true);
			actionPause->setEnabled (false);
			actionKill->setEnabled (true);
			mStatusLabel->setText (tr("Target stopped"));
		break;
	}
}
