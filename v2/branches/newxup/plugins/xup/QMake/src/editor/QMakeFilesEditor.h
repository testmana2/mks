#ifndef QMAKEFILESEDITOR_H
#define QMAKEFILESEDITOR_H

#include <FilesEditor.h>

#include "UIQMakeEditor.h"

class QMakeFilesEditor : public FilesEditor
{
	Q_OBJECT
	
public:
	QMakeFilesEditor( UIQMakeEditor::ProjectValues& positive, UIQMakeEditor::ProjectValues& negative, QWidget* parent = 0 );
	virtual ~QMakeFilesEditor();

protected:
	UIQMakeEditor::ProjectValues& mPositiveValues;
	UIQMakeEditor::ProjectValues& mNegativeValues;
};

#endif // QMAKEFILESEDITOR_H
