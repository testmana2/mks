#ifndef UIQMAKEEDITOR_H
#define UIQMAKEEDITOR_H

#include <UIXUPEditor.h>

#include <QHash>

class UIQMakeEditor : public UIXUPEditor
{
	Q_OBJECT
	
public:
	typedef QHash<QString, QStringList> ProjectValues;
	UIQMakeEditor( QWidget* parent = 0 );
	virtual ~UIQMakeEditor();
	
	virtual void setupProject( XUPProjectItem* project );
	virtual bool showProjectFilesPage();

protected:
	UIQMakeEditor::ProjectValues mPositiveValues;
	UIQMakeEditor::ProjectValues mNegativeValues;
	
	virtual void finalize();
	
	void initializeVariables( XUPProjectItem* project );
};

#endif // UIQMAKEEDITOR_H
