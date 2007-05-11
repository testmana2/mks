#ifndef QMAKEPROJECTPARSER_H
#define QMAKEPROJECTPARSER_H
//
#include <QObject>
//
class QMakeProjectItem;
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
	QStringList explodeValue( const QString& );
	bool loadFile( const QString&, QMakeProjectItem* );
	int parseBuffer( int, QMakeProjectItem* );
	void fakeProject( QMakeProjectItem* );
	//
	bool mIsOpen;
	QMakeProjectItem* mRoot;
	//
};
//
#endif // QMAKEPROJECTPARSER_H
