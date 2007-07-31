/*****************************************************
* AUTHOR   : Nox P@sNox <pasnox@gmail.com>
* PROJECT  : 
* FILENAME : pTools.h
* DATE     : 2007/07/29
* TIME     : 13:13
* LICENSE  : GPL
* COMMENT  : Your comment here
*****************************************************/
#ifndef PTOOLS_H
#define PTOOLS_H

#include <QApplication>
#include <QMessageBox>

namespace pTools
{
	void warning( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	void information( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	bool question( const QString&, const QString&, QWidget* = QApplication::activeWindow(), QMessageBox::StandardButtons = QMessageBox::Yes | QMessageBox::No, QMessageBox::StandardButton = QMessageBox::No );

	const QStringList availableImageFormats();

	const QString getImageFileName( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	const QString getOpenFileName( const QString&, const QString&, QWidget* = QApplication::activeWindow() );
	const QString getExistingDirectory( const QString&, const QString&, QWidget* = QApplication::activeWindow() );

	const QString tokenizeHome( const QString& );
	const QString unTokenizeHome( const QString& );

};

#endif // PTOOLS_H
