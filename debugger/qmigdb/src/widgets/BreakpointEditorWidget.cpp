#include "BreakpointEditorWidget.h"

#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>

BreakpointEditorWidget::BreakpointEditorWidget( const QGdb::Breakpoint& breakpoint, QWidget* parent )
	: QDialog( parent )
{
	mBreakpoint = breakpoint;
	
	setWindowTitle( tr( "Edit breakpoint" ) );
	
	// condition
	gbCondition = new QGroupBox( tr( "Condition" ), this );
	gbCondition->setFlat( true );
	gbCondition->setCheckable( true );
	gbCondition->setChecked( !mBreakpoint.condition.isEmpty() );
	
	leCondition = new QLineEdit( this );
	leCondition->setText( mBreakpoint.condition );
	
	QVBoxLayout* vlCondition = new QVBoxLayout( gbCondition );
	vlCondition->addWidget( leCondition );
	
	// ignore
	gbIgnore = new QGroupBox( tr( "Ignore" ), this );
	gbIgnore->setFlat( true );
	gbIgnore->setCheckable( true );
	gbIgnore->setChecked( mBreakpoint.ignore );
	
	sbIgnore = new QSpinBox( this );
	sbIgnore->setMaximum( 999999999 );
	sbIgnore->setValue( mBreakpoint.times );
	
	QVBoxLayout* vlIgnore = new QVBoxLayout( gbIgnore );
	vlIgnore->addWidget( sbIgnore );
	
	// enabled
	gbEnabled = new QGroupBox( tr( "Enabled" ), this );
	gbEnabled->setFlat( true );
	gbEnabled->setCheckable( true );
	gbEnabled->setChecked( mBreakpoint.enabled );
	
	QVBoxLayout* vlEnabled = new QVBoxLayout( gbEnabled );
	vlEnabled->addWidget( gbCondition );
	vlEnabled->addWidget( gbIgnore );
	
	// buttons
	QDialogButtonBox* dbb = new QDialogButtonBox( this );
	dbb->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
	
	connect( dbb, SIGNAL( accepted() ), this, SLOT( accept() ) );
	connect( dbb, SIGNAL( rejected() ), this, SLOT( reject() ) );
	
	// main layout
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->addWidget( gbEnabled );
	vl->addWidget( dbb );
}

BreakpointEditorWidget::~BreakpointEditorWidget()
{
}

QGdb::Breakpoint BreakpointEditorWidget::editedBreakpoint() const
{
	return mBreakpoint;
}

void BreakpointEditorWidget::accept()
{
	mBreakpoint.condition = leCondition->text();
	if ( !gbCondition->isChecked() )
	{
		mBreakpoint.condition.clear();
	}
	
	mBreakpoint.ignore = gbIgnore->isChecked();
	mBreakpoint.times = sbIgnore->value();
	if ( !gbIgnore->isChecked() )
	{
		mBreakpoint.times = 0;
	}
	
	mBreakpoint.enabled = gbEnabled->isChecked();
	
	QDialog::accept();
}
