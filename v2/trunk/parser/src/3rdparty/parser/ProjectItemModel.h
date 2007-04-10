#ifndef PROJECTITEMMODEL_H
#define PROJECTITEMMODEL_H

#include <QStandardItemModel>
#include <QStringList>
#include <QTextStream>
#include <QHash>

//<variable, <operator, pointer>>
typedef QHash< QString, QHash<QString, uint> > HashSimpleSettings;
//<project item pointer, HashSimpleSettings>
typedef QHash<uint, HashSimpleSettings> ProjectList;

class ProjectItem;

class ProjectItemModel : public QStandardItemModel
{
	Q_OBJECT
public:
	ProjectItemModel();
	//
	bool openFile(QString&);
	ProjectItem* currentProject( const QModelIndex& = QModelIndex());
	QString currentProjectName( const QModelIndex& = QModelIndex() );
	QString currentProjectPath();
	QStringList getSettingsList(ProjectItem*);
	void setSettingsList(ProjectItem*, const QStringList&);
	ProjectItem* addNewItem(const QString&, const QString&);
	HashSimpleSettings simpleSettings(const QModelIndex& = QModelIndex());
	// pasnox
	void updateItem( ProjectItem* );
public slots:
	void saveProject();
private:
	HashSimpleSettings initializeSimpleSettings();
	void clearHash(ProjectItem* item);
	bool parseFile(const QString&, ProjectItem* = 0);
	void writeToBuffer(QList<ProjectItem*> = QList<ProjectItem*>());
	void writeToFile();
	ProjectItem* createProjectItem(const QString&, ProjectItem* = 0);
	void parseLine(const QString&, ProjectItem* = 0);
	void parseBlock(const QString&, ProjectItem* = 0);
	void parseText(QTextStream&, ProjectItem* = 0);
	void appendItem(ProjectItem*, ProjectItem* = 0);
	void setItemData(ProjectItem*);
	void removeChilds(ProjectItem*);
	//
	//QString getValue( const QString&, const QStringList& = QStringList());
	//QString getValues( const QString&, const QStringList& = QStringList());
	QStringList getValuesList( const QString&, const QStringList& = QStringList());
	//
	/*void setValue( const QString&, const QString&, const QStringList& = QStringList());
	void setValues( const QString&, const QString&, const QStringList& = QStringList());
	void setValuesList( const QStringList&, const QString&, const QStringList& = QStringList());*/
	//
	QStringList absoluteSubProjectsPath(ProjectItem* = 0);
	QStringList getStringList(const QString&);
	QList<ProjectItem*> findItem( const QString&, const QStringList& = QStringList(), const QString& = QString());
	//
	ProjectItem* getFolder(const QString&, ProjectItem*);
	QString checkForComment(QString, ProjectItem* = 0);
	QStringList fileNameBasedVariables();
	//
	int findCloseBracketFor(const QChar&, const QString&);
	int findOpenBracketFor(const QChar&, const QString&);
	//
	QList<ProjectItem*> complexScopeList;
	QList<ProjectItem*> topLevelItems();
	//
	ProjectItem* mProject;
	ProjectList mProjectList;
	QString mFileText;
	int indent;
signals:
	void sig_writeText(const QString&);
};

#endif //PROJECTITEMMODEL_H