#ifndef REPLACETHREAD_H
#define REPLACETHREAD_H

#include <QThread>
#include <QMutex>

#include "SearchResultsModel.h"

class ReplaceThread : public QThread
{
	Q_OBJECT
	
public:
	ReplaceThread( QObject* parent = 0 );
	virtual ~ReplaceThread();
	
	void replace( const SearchWidget::Properties& properties, const QHash<QString, SearchResultsModel::ResultList>& results );
	void stop();

protected:
	static int mMaxTime;
	SearchWidget::Properties mProperties;
	QHash<QString, SearchResultsModel::ResultList> mResults;
	QMutex mMutex;
	bool mReset;
	bool mExit;
	
	void saveContent( const QString& fileName, const QString& content, const QString& codec );
	QString fileContent( const QString& fileName ) const;
	void replace( const QString& fileName, QString content );
	void run();

signals:
	void resultsHandled( const QString& fileName, const SearchResultsModel::ResultList& results );
	void openedFilesHandled( const QHash<QString, QString>& filesContent );
	void error( const QString& error );
};

#endif // REPLACETHREAD_H
