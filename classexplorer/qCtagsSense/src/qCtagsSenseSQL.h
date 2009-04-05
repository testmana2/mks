#ifndef QCTAGSSENSESQL_H
#define QCTAGSSENSESQL_H

#include <QObject>
#include <QtSql>

class qCtagsSenseSQL : public QObject
{
	Q_OBJECT
	
public:
	qCtagsSenseSQL( QObject* parent = 0 );
	virtual ~qCtagsSenseSQL();
	
	bool initializeDatabase();
	bool initializeTables() const;
	
	QSqlDatabase database() const;
	QSqlQuery query() const;

protected:
	QString mDBConnectionName;
	static const QString mDBFilePath;
};

#endif // QCTAGSSENSESQL_H