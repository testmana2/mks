#ifndef PROJECTITEMDELEGATE_H
#define PROJECTITEMDELEGATE_H
//
#include <QItemDelegate>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <QDir>
//
#include "ProjectItem.h"
#include "ProjectItemModel.h"
//
class ProjectItemDelegate : public QItemDelegate
{
	Q_OBJECT
	//
public:
	ProjectItemDelegate( QObject* = 0 );
	//
	virtual QWidget* createEditor( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const;
	virtual void setModelData( QWidget*, QAbstractItemModel*, const QModelIndex& ) const;
};
//
class QValueFileFolderEdit : public QWidget
{
	Q_OBJECT
private:
	QComboBox* cbValue;
	QPushButton* pbFile;
	QPushButton* pbFolder;
	QString ProjectPath;
	//
public:
	QValueFileFolderEdit( QWidget* p, const QModelIndex& i )
		: QWidget( p )
	{
		QHBoxLayout* hl = new QHBoxLayout( this );
		hl->setMargin( 0 );
		hl->setSpacing( 0 );
		hl->addWidget( ( cbValue = new QComboBox( this ) ), 100 );
		hl->addWidget( ( pbFile = new QPushButton( this ) ) );
		hl->addWidget( ( pbFolder = new QPushButton( this ) ) );
		//
		cbValue->setEditable( true );
		pbFile->setIcon( QIcon( ":/Icons/Icons/fileopen.png" ) );
		pbFile->setToolTip( tr( "Set as file..." ) );
		pbFolder->setIcon( QIcon( ":/Icons/Icons/folder.png" ) );
		pbFolder->setToolTip( tr( "Set as folder..." ) );
		// set the project path if available
		if ( i.isValid() )
		{
			ProjectItem* mItem = reinterpret_cast<ProjectItem*>( i.model()->index( i.model()->rowCount() -1, 1 ).data( Qt::DisplayRole ).toUInt() );
			if ( mItem )
			{
				ProjectPath = QFileInfo( mItem->data( ProjectItem::AbsoluteFilePathRole ).toString() ).canonicalPath();
				if ( !ProjectPath.endsWith( "/" ) )
					ProjectPath.append( "/" );
			}
		}
		//
		connect( pbFile, SIGNAL( clicked() ), this, SLOT( pbClicked() ) );
		connect( pbFolder, SIGNAL( clicked() ), this, SLOT( pbClicked() ) );
	}
	//
	QString text() const
	{
		return cbValue->currentText();
	}
	//
	QString getScope( const QString& t, const QString& l, const QString& v )
	{
		QStringList scopes;
		scopes << "win32" << "macx" << "unix" << "qt" << "win32-g++" << "macx-g++" << "linux-g++";
		bool b;
		QString s = QInputDialog::getItem( this, t, l, scopes, scopes.indexOf( v ), true, &b );
		if ( b && !s.isEmpty() )
			return s;
		return QString::null;
	}
	//
	QString getFolder( const QString& t, const QString& d )
	{
		return QFileDialog::getExistingDirectory( this, t, d );
	}
	//
	QString getFile( const QString& t, const QString& d )
	{
		return QFileDialog::getOpenFileName( this, t, d );
	}
	//
public slots:
	void setText( const QString& s )
	{
		if ( cbValue->findText( s ) == -1 )
			cbValue->addItem( s );
		cbValue->setCurrentIndex( cbValue->findText( s ) );
	}
	//
	void pbClicked()
	{
		QPushButton* pb = qobject_cast<QPushButton*>( sender() );
		QString s = text();
		if ( !ProjectPath.isNull() )
			s.prepend( ProjectPath );
		if ( pb == pbFile )
			s = getFile( "Change File...", QDir::cleanPath( s ) );
		else
			s = getFolder( "Change Folder...", QDir::cleanPath( s ) );
		if ( !s.isNull() )
		{
			if ( !ProjectPath.isNull() )
			{
				QDir d( ProjectPath );
				s = d.relativeFilePath( s );
			}
			setText( s );
		}
	}
};
//
#endif // PROJECTITEMDELEGATE_H
