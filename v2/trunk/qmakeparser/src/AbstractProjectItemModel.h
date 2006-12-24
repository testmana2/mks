#ifndef ABSTRACTPROJECTITEMMODEL_H
#define ABSTRACTPROJECTITEMMODEL_H
//
#include <QStandardItemModel>
//
class AbstractProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	//
public:
	AbstractProjectItemModel( const QString&, QObject* = 0 );
	//
	virtual bool isOpen() const;
	virtual QString name() const;
	virtual QString path() const;
	virtual QString filePath() const;
	virtual QString filePath( const QString& );
	virtual bool open( bool = true ) = 0;
	virtual QStandardItem* projectItem() const = 0;
	//
protected:
	virtual bool parse() = 0;
	//
	QString mFilePath;
	bool mIsOpen;
	//
public slots:
	virtual void setFilePath( const QString& );
	virtual void close() = 0;
	//
signals:
	void isOpenChanged( bool );
	void filePathChanged( const QString& );
	//
};
//
#endif // ABSTRACTPROJECTITEMMODEL_H
