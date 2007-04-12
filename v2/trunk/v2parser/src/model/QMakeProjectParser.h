#ifndef QMAKEPROJECTPARSER_H
#define QMAKEPROJECTPARSER_H
//
#include <QBuffer>
//
#include "QMakeProjectItem.h"
//
class QMakeProjectParser
{
public:
	QMakeProjectParser( const QBuffer&, QMakeProjectItem* );
	~QMakeProjectParser();
	//
};
//
#endif // QMAKEPROJECTPARSER_H
