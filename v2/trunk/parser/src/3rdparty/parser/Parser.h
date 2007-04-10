#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QStringList>
#include <QTextStream>

class ProjectItem;

class Parser : public QObject
{
	Q_OBJECT
public:
	Parser();
	//
	bool parseFile(const QString&);
	void writeToBuffer(QList<ProjectItem*> = QList<ProjectItem*>());
	void writeToFile();
private:
	void parseLine(const QString&, ProjectItem* = 0);
	void parseBlock(const QString&, ProjectItem* = 0);
	void parseSimpleBlock(const QString&, ProjectItem* = 0);
	void parseText(QTextStream&, ProjectItem* = 0);
	void appendItem(ProjectItem*, ProjectItem* = 0);
	QString checkForComment(QString, ProjectItem* = 0);
	//
	QList<ProjectItem*> complexScopeList;
	QList<ProjectItem*> mProjectItemsList;
	//
	QString mFileText;
	int indent;
};

#endif //PARSER_H