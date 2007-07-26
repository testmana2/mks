#include "pTemplatePreviewer.h"
#include "pEditor.h"
#include "pQScintilla.h"

#include <QVariant>
#include <QLabel>
#include <QVBoxLayout>

pTemplatePreviewer::pTemplatePreviewer( QWidget* w )
	: QWidget( w )
{
	QVBoxLayout* mLayout = new QVBoxLayout( this );
	mLabel = new QLabel;
	mEditor = new pEditor;
	mLayout->addWidget( mLabel );
	mLayout->addWidget( mEditor );
}

pTemplatePreviewer::~pTemplatePreviewer()
{
}

void pTemplatePreviewer::setFileName( const QString& s )
{
	mLabel->setText( s );
}

QString pTemplatePreviewer::fileName() const
{
	return mLabel->text();
}

void pTemplatePreviewer::setDestination( const QString& s )
{
	setProperty( "pdestination", s );
}

QString pTemplatePreviewer::destination() const
{
	return property( "pdestination" ).toString();
}

void pTemplatePreviewer::setOpen( bool b )
{
	setProperty( "pOpen", b );
}

bool pTemplatePreviewer::open() const
{
	return property( "pOpen" ).toBool();
}

bool pTemplatePreviewer::open( const QString& s )
{
	setOpen( mEditor->openFile( s ) );
	return open();
}

pEditor* pTemplatePreviewer::editor() const
{
	return mEditor;
}
