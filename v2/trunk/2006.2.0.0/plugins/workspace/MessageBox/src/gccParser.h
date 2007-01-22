#ifndef GCCPARSER_H
#define GCCPARSER_H
#include <QObject>
#include <QString>
#include <QVector>
#include <QListWidgetItem>
#include<QIcon>
//


enum errType {error, warning};
struct errStruct
{
	QString fileName;
	int line;
	errType type;
	QString text;
	QString fullText;
};

class gccParser : public QObject  {
Q_OBJECT
private:
	QVector <errStruct*> errPointers;
	QString regExp;
	QString buffer;
public:
	gccParser();
public slots:
	QVector<errStruct*>* parseString (QString);
	void setRegExp (QString);
};
#endif // GCCPARSER_H
