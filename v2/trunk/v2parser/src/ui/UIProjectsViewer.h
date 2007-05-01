#ifndef UIPROJECTSVIEWER_H
#define UIPROJECTSVIEWER_H
//
#include "ui_UIProjectsViewer.h"
//
#include <QPointer>
//
class QMakeProjectModel;
class QMakeProjectProxy;
//
class UIProjectsViewer : public QWidget, public Ui::UIProjectsViewer
{
	Q_OBJECT
	//
public:
	static UIProjectsViewer* self( QWidget* = 0 );
	bool openProject( const QString& );
	//
protected:
	UIProjectsViewer( QWidget* = 0 );
	static QPointer<UIProjectsViewer> mSelf;
	QMakeProjectModel* mModel;
	QMakeProjectProxy* mProxy;
	//
protected slots:
	void on_pbSettings_clicked();
	//
};
//
#endif // UIPROJECTSVIEWER_H
