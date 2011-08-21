#ifndef FILESEDITOR_H
#define FILESEDITOR_H

#include "MonkeyExport.h"
#include "XUPPageEditor.h"

#include <QModelIndex>

class Ui_FilesEditor;

class XUPProjectItem;
class XUPItem;
class XUPItemVariableEditorModel;

class Q_MONKEY_EXPORT FilesEditor : public XUPPageEditor
{
	Q_OBJECT

public:
	FilesEditor( QWidget* parent = 0 );
	virtual ~FilesEditor();
	
	bool isQuoteSpacedValuesVisible() const;
	bool isQuoteSpacedValuesEnabled() const;
	bool isQuoteSpacedValuesChecked() const;
	QString quoteSpacedValuesString() const;
	
	bool isDeleteRemovedFilesVisible() const;
	bool isDeleteRemovedFilesEnabled() const;
	bool isDeleteRemovedFilesChecked() const;
	
	void setup( XUPProjectItem* project );
	void finalize();

public slots:
	void setQuoteSpacedValuesVisible( bool visible );
	void setQuoteSpacedValuesEnabled( bool enabled );
	void setQuoteSpacedValuesChecked( bool checked );
	void setQuoteSpacedValuesString( const QString& string );
	
	void setDeleteRemovedFilesVisible( bool visible );
	void setDeleteRemovedFilesEnabled( bool enabled );
	void setDeleteRemovedFilesChecked( bool checked );

protected:
	Ui_FilesEditor* ui;
	XUPProjectItem* mProject;
	XUPItemVariableEditorModel* mModel;
	
	XUPItem* variableItem( const QString& variableName, bool create );
	QModelIndex currentVariable() const;
	QModelIndex currentValue() const;

protected slots:
	void tvVariables_selectionModel_selectionChanged();
	void on_tbAdd_clicked();
	void on_tbEdit_clicked();
};

#endif // FILESEDITOR_H
