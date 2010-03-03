#ifndef PCOMPLETERINDEXER_H
#define PCOMPLETERINDEXER_H

#include <QObject>

#include "XUPProjectItem.h"

class pCompleterIndexer : public QObject
{
	Q_OBJECT
	
public:
	static pCompleterIndexer* instance();
	virtual ~pCompleterIndexer();
	
protected slots:
	void opened(XUPProjectItem*);
	
private:
	pCompleterIndexer();
	void recursiveFileEntries(const QStringList&, const QString&, QStringList* list);
	
	static pCompleterIndexer* pInstance;
};

#endif // PCOMPLETERINDEXER_H
