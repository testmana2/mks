#ifndef UISIMPLEQMAKEEDITOR_H
#define UISIMPLEQMAKEEDITOR_H

#include "ui_UIXUPEditor.h"

class XUPPageEditor;
class XUPProjectItem;

class UIXUPEditor : public QDialog, public Ui::UIXUPEditor
{
	Q_OBJECT
	
public:
	UIXUPEditor( XUPProjectItem* project, QWidget* parent = 0 );
	virtual ~UIXUPEditor();
	
	int insertPage( int index, XUPPageEditor* page );
	int addPage( XUPPageEditor* page );
	void addPages( const QList<XUPPageEditor*>& pages );
	XUPPageEditor* page( int index ) const;
	void removePage( int index );
	
	QList<XUPPageEditor*> pages() const;
	void setCurrentPage( int index );
	int currentPage() const;
	
	void defaultSetup( XUPProjectItem* project );

protected:
	XUPProjectItem* mProject;
	QList<XUPPageEditor*> mPages;
	
	void setup( XUPProjectItem* project );
	void finalize();

protected slots:
	void accept();
};

#endif // UISIMPLEQMAKEEDITOR_H
