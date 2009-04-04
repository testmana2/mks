#ifndef QCTAGSSENSEBROWSER_H
#define QCTAGSSENSEBROWSER_H

#include "ui_qCtagsSenseBrowser.h"

class qCtagsSense;
class qCtagsSenseLanguagesModel;
class qCtagsSenseFilesModel;
class qCtagsSenseMembersModel;

class qCtagsSenseBrowser : public QFrame, public Ui::qCtagsSenseBrowser
{
	Q_OBJECT

public:
	qCtagsSenseBrowser( QWidget* parent = 0 );
	virtual ~qCtagsSenseBrowser();

protected:
	qCtagsSense* mSense;
	qCtagsSenseLanguagesModel* mLanguagesModel;
	qCtagsSenseFilesModel* mFilesModel;
	qCtagsSenseMembersModel* mMembersModel;
	
	class FileManager* mFileManager;

protected slots:
	void on_cbLanguages_currentIndexChanged( int id );
	void on_cbFileNames_currentIndexChanged( int id );
	void on_cbMembers_currentIndexChanged( int id );
	void on_tvMembers_activated( const QModelIndex& index );
	void indexChanged();

signals:
	void memberActivated( const QString& fileName, const QModelIndex& index );
};

#endif // QCTAGSSENSEBROWSER_H
