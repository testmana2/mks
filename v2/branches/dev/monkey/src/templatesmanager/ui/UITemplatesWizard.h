#ifndef UITEMPLATESWIZARD
#define UITEMPLATESWIZARD

#include "MonkeyExport.h"
#include "QSingleton.h"
#include "ui_UITemplatesWizard.h"
#include "pTemplatesManager.h"

class ProjectsModel;

class Q_MONKEY_EXPORT UITemplatesWizard : public QDialog, public Ui::UITemplatesWizard, public QSingleton<UITemplatesWizard>
{
	friend class QSingleton<UITemplatesWizard>;
    Q_OBJECT
    
private:
	void setType(QString type);
    TemplateList mTemplates;
    ProjectsModel* mProjects;
    UITemplatesWizard( QWidget* = 0 );
    QStringList getLanguages ();
    QStringList getTypes ();

private slots:
    void on_cbLanguages_currentIndexChanged( const QString& );
    void on_cbTypes_currentIndexChanged( const QString& );
    void on_tbDestination_clicked();
#if 0
    void on_swPages_currentChanged( int );
    void on_pbPrevious_clicked();
    void on_pbNext_clicked();
#endif
//    bool checkTemplate();
//    void generatePreview();
    void on_accept();
};

#endif // UITEMPLATESWIZARD
 
