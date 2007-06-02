#ifndef QMAKEPROJECTPARSER_H
#define QMAKEPROJECTPARSER_H
//
#include <QObject>
//
class QMakeProjectItem;
class QMakeProjectModel;
//
class QMakeProjectParser : public QObject
{
	Q_OBJECT
	//
public:
	QMakeProjectParser( const QString&, QMakeProjectItem* );
	~QMakeProjectParser();
	//
	bool isOpen() const;
	//
protected:
	bool loadFile( const QString&, QMakeProjectItem* );
	int parseBuffer( int, QMakeProjectItem* );
	//
	QMakeProjectItem* processNested( const QString&, QMakeProjectItem* );
	QMakeProjectItem* processValues( const QString&, QMakeProjectItem* );
	QMakeProjectItem* addScope( const QString&, const QString&, bool, QMakeProjectItem* );
	QMakeProjectItem* addVariable( const QString&, const QString&, QMakeProjectItem* );
	QMakeProjectItem* addFunction( const QString&, const QString&, QMakeProjectItem* );
	QMakeProjectItem* addValue( const QString&, QMakeProjectItem* );
	QMakeProjectItem* addComment( const QString&, QMakeProjectItem* );
	QMakeProjectItem* addEmpty( QMakeProjectItem* );
	//
	bool mIsOpen;
	QMakeProjectItem* mRoot;
	QMakeProjectModel* mModel;
	//
};
//
#endif // QMAKEPROJECTPARSER_H
