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

void pTemplatePreviewer::setText( const QString& s )
{
	mLabel->setText( s );
}

QString pTemplatePreviewer::text() const
{
	mLabel->text();
}

void pTemplatePreviewer::setLanguage( const QString& s )
{
	setProperty( "pLanguage", s );
}

QString pTemplatePreviewer::language() const
{
	return property( "pLanguage" ).toString();
}

void pTemplatePreviewer::setName( const QString& s )
{
	setProperty( "pName", s );
}

QString pTemplatePreviewer::name() const
{
	return property( "pName" ).toString();
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
