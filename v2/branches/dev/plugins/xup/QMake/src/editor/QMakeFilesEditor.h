#ifndef QMAKEFILESEDITOR_H
#define QMAKEFILESEDITOR_H

#include <xupmanager/gui/FilesEditor.h>

class QMakeFilesEditor : public FilesEditor
{
    Q_OBJECT
    
public:
    QMakeFilesEditor( QWidget* parent = 0 );
    virtual ~QMakeFilesEditor();
    
    virtual QStringList filteredFileVariables() const;
};

#endif // QMAKEFILESEDITOR_H
