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
	QList <QLabel*> mLabels;
	QList <QComboBox*> mCombos;


	void setType(QString type);
    TemplateList mTemplates;
    ProjectsModel* mProjects;
    UITemplatesWizard( QWidget* = 0 );
    QStringList getLanguages ();
    QStringList getTypes ();

private slots:
    void onFiltersChanged ();
	void onTemplateSelected (QListWidgetItem*);
    void on_tbDestination_clicked();

    bool checkTemplate();
    void on_pbCreate_clicked();
};

#endif // UITEMPLATESWIZARD
 
