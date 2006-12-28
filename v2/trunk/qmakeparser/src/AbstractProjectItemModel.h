#ifndef ABSTRACTPROJECTITEMMODEL_H
#define ABSTRACTPROJECTITEMMODEL_H
//
#include <QStandardItemModel>
//
class AbstractProjectItemModel;
typedef QHash<int, AbstractProjectItemModel*> QHashProjects; // id, project
//
class AbstractProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
	//
public:
	AbstractProjectItemModel( const QString&, QObject* = 0 );
	//
	int id() const;
	static AbstractProjectItemModel* byId( int );
	static QHashProjects all();
	//
	virtual bool isOpen() const;
	virtual bool isModified() const;
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
	static int mUniqueId;
	int mId;
	static QHashProjects mProjectsList;
	QString mFilePath;
	bool mIsOpen;
	bool mIsModified;
	//
public slots:
	virtual void setFilePath( const QString& );
	virtual void setModified( bool );
	virtual void close() = 0;
	virtual void save() = 0;
	//
signals:
	void isOpenChanged( bool );
	void isModifiedChanged( bool );
	void filePathChanged( const QString& );
	//
};
//
#endif // ABSTRACTPROJECTITEMMODEL_H
