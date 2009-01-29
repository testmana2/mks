#include "UIMonkeyDbg.h"

UIMonkeyDbg::UIMonkeyDbg( QWidget* parent )
	: QMainWindow( parent )
{
	setupUi( this );
}

UIMonkeyDbg::~UIMonkeyDbg()
{
}

bool UIMonkeyDbg::openFile( const QString& fileName )
{
}

void UIMonkeyDbg::closeCurrentFile()
{
}

void UIMonkeyDbg::closeAllFiles()
{
}

bool UIMonkeyDbg::loadTarget( const QString& fileName )
{
}

void UIMonkeyDbg::debuggerRun()
{
}

void UIMonkeyDbg::debuggerContinue()
{
}

void UIMonkeyDbg::debuggerStepInto( bool instruction )
{
}

void UIMonkeyDbg::debuggerStepOver( bool instruction )
{
}

void UIMonkeyDbg::debuggerStepOut()
{
}

void UIMonkeyDbg::debuggerStop()
{
}

void UIMonkeyDbg::debuggerKill()
{
}

void UIMonkeyDbg::on_aOpenFile_triggered()
{
}

void UIMonkeyDbg::on_aLoadTarget_triggered()
{
}
