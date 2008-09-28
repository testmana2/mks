#ifndef QMAKE2XUP_H
#define QMAKE2XUP_H

#include <QDomDocument>

namespace QMake2XUP
{
	QString convertFromPro( const QString& fileName, const QString& encoding = QLatin1String( "UTF-8" ) );
	QString convertToPro( const QDomDocument& project, const QString& encoding = QLatin1String( "UTF-8" ) );
};

#endif // QMAKE2XUP_H
