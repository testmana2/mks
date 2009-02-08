#ifndef BREAKPOINTEDITORWIDGET_H
#define BREAKPOINTEDITORWIDGET_H

#include <QDialog>

#include "QGdbBreakpoint.h"

class QGroupBox;
class QLineEdit;
class QSpinBox;

class BreakpointEditorWidget : public QDialog
{
	Q_OBJECT
	
public:
	BreakpointEditorWidget( const QGdb::Breakpoint& breakpoint, QWidget* parent = 0 );
	virtual ~BreakpointEditorWidget();
	
	QGdb::Breakpoint editedBreakpoint() const;

protected:
	QGdb::Breakpoint mBreakpoint;
	QGroupBox* gbEnabled;
	QGroupBox* gbCondition;
	QLineEdit* leCondition;
	QGroupBox* gbIgnore;
	QSpinBox* sbIgnore;

protected slots:
	void accept();
};

#endif // BREAKPOINTEDITORWIDGET_H
