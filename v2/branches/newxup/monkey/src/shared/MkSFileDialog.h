#ifndef MKSFILEDIALOG_H
#define MKSFILEDIALOG_H

#include "pFileDialog.h"

class XUPAddFiles;
class XUPItem;

class Q_MONKEY_EXPORT MkSFileDialog : public pFileDialog
{
	Q_OBJECT
	
public:
	MkSFileDialog( QWidget* parent = 0, const QString& caption = QString(), const QString& directory = QString(), const QString& filter = QString(), bool textCodecEnabled = true );
	
	void setRecentNameFilter( const QString& name );
	
	static pFileDialogResult getOpenFileName( bool useRecents, QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, QString* selectedFilter = 0, Options options = 0 );
	static pFileDialogResult getOpenFileNames( bool useRecents, QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, QString* selectedFilter = 0, Options options = 0 );
	
	static pFileDialogResult getSaveFileName( bool useRecents, QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QString& filter = QString(), bool enabledTextCodec = true, QString* selectedFilter = 0, Options options = 0 );
	static pFileDialogResult getExistingDirectory( bool useRecents, QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), bool enabledTextCodec = true, Options options = ShowDirsOnly );
	
	//static pFileDialogResult getProjectAddFiles( QWidget* parent = 0, bool allowChooseScope = true );
	static pFileDialogResult getNewEditorFile( QWidget* parent = 0 );

protected:
	XUPAddFiles* mAddFiles;

protected slots:
	void currentScopeChanged( XUPItem* scope );
};

#endif // MKSFILEDIALOG_H
