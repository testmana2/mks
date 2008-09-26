#ifndef PICONMANAGER_H
#define PICONMANAGER_H

#include <QPixmapCache>
#include <QCache>
#include <QIcon>

typedef QCache<QString, QIcon> QIconCache;

class pIconManager
{
public:
	static QString filePath( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	static QPixmap pixmap( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );
	static QIcon icon( const QString& fileName, const QString& prefix = QLatin1String( ":/" ) );

protected:
	static QIconCache mIconCache;
};

#endif // PICONMANAGER_H
