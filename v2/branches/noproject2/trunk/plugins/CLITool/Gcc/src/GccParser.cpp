#include "GccParser.h"
#include <QDebug>

GccParser::GccParser()
{
}

GccParser::~GccParser()
{
}


bool GccParser::parse(const QByteArray* arr)
{
	qDebug()<<"trying to parse "<< QString (*arr);
	return false;
}

QString GccParser::name() const
{
	return QString ("Gcc");
}
