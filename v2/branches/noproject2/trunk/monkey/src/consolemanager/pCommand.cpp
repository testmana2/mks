#include "pCommand.h"

QString pCommand::command() const
{
	//FIXME there is code, determining command using plugins
	return mDefaultCommand;
}