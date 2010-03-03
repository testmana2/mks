#ifndef PCOMPLETER_H
#define PCOMPLETER_H

#include <QString>
#include <QPoint>
#include <QStringListModel>

#include "qCtagsSenseSQL.h"
#include "qCtagsSense.h"
#include "qCtagsSenseUtils.h"

class pCompleter
{
public:
	pCompleter(QString fileName, QPoint cursorPos);
	
	void init();
	void setCursorPos(QPoint cursorPos);
	bool isPointer(QString);
	QStringListModel* autoComplete(QString);
	QString getStrToAnalyze();
	virtual ~pCompleter();
	
private:
	qCtagsSenseSQL* sqlBase;
	
	QString filename;
	QPoint cursorPos;
	bool inProcess;
	bool bIsPointer;
	QString strToAnalyze;
	QString lineToAnalyze;
	qCtagsSense::Kind typeToAnalyze;
	QString preciseType;
	
	void parse(QString);
	void analyze();
	bool variableRange(quint32, quint32);
	void patternMatching(QString, QString);
	QString lineAt(int index, QString filename = "");
};

#endif // PCOMPLETER_H
