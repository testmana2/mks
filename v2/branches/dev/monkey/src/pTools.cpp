/*****************************************************
* AUTHOR   : Nox P@sNox <pasnox@gmail.com>
* PROJECT  : 
* FILENAME : pTools.cpp
* DATE     : 2007/07/29
* TIME     : 13:13
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#include "pTools.h"
#include "pSettings.h"

#include <QImageReader>
#include <QFileDialog>
#include <QDir>

void pTools::warning( const QString& c, const QString& s, QWidget* w )
{
	QMessageBox::warning( w, c, s );
}

void pTools::information( const QString& c, const QString& s, QWidget* w )
{
	QMessageBox::information( w, c, s );
}

bool pTools::question( const QString& c, const QString& s, QWidget* w, QMessageBox::StandardButtons b, QMessageBox::StandardButton d )
{
	if ( QMessageBox::question( w, c, s, b, d ) == QMessageBox::Yes )
		return true;
	return false;
}

const QStringList pTools::availableImageFormats()
{
	QStringList l;
	foreach ( QByteArray a, QImageReader::supportedImageFormats() )
		l << a;
	return l;
}

const QString pTools::getImageFileName( const QString& c, const QString& f, QWidget* w )
{
	// get image types
	QStringList l;
	foreach ( QString s, availableImageFormats() )
		l << QObject::tr( "%1 Files (*.%2)" ).arg( s.toUpper() ).arg( s );

	// create dialg
	QFileDialog d( w, c.isEmpty() ? QObject::tr( "Select an image" ) : c, f, l.join( ";;" ) );
	d.setAcceptMode( QFileDialog::AcceptOpen );
	d.setFileMode( QFileDialog::ExistingFile );
	// choose last used filter if available
	d.selectFilter( pSettings::instance()->value( "Recents/ImageFilter" ).toString() );
	// set filename
	QFileInfo fi( f );
	d.setDirectory( fi.path() );
	d.selectFile( fi.fileName() );

	// execute dialog
	d.exec();

	// remember last filter used
	pSettings::instance()->setValue( "Recents/ImageFilter", d.selectedFilter() );
	
	// return filename
	if ( d.result() == QDialog::Accepted )
		return d.selectedFiles().value( 0 );
	return QString();
}

const QString pTools::getOpenFileName( const QString& c, const QString& f, QWidget* w )
{
	return QFileDialog::getOpenFileName( w, c, f );
}

const QString pTools::getExistingDirectory( const QString& c, const QString& f, QWidget* w )
{
	return QFileDialog::getExistingDirectory( w, c, f );
}

const QString pTools::tokenizeHome( const QString& s )
{
	return QString( s ).replace( QDir::homePath(), "$HOME$" ); 
}

const QString pTools::unTokenizeHome( const QString& s )
{
	return QString( s ).replace( "$HOME$", QDir::homePath() );
}
