#include "QMakeProjectItemModel.h"
#include "QMakeProjectItem.h"
//
#include <QTime>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QRegExp>
//
#include <qscilexercpp.h>
#include <qsciapis.h>
//
static QRegExp rx;
//
QMakeProjectItemModel::QMakeProjectItemModel( const QString& s, QObject* p )
	: AbstractProjectItemModel( s, p )
{
	mLexer = new QsciLexerCPP( this );
	mAPIs = new QsciAPIs( mLexer ) ;
}
//
QMakeProjectItemModel::~QMakeProjectItemModel()
{
	// close project on delete
	close();
}
//
QStringList QMakeProjectItemModel::simpleModelVariables()
{
	return QStringList() << "FORMS" << "HEADERS" << "SOURCES"
		<< "TRANSLATIONS" << "RESOURCES" << "OPENEDFILES";
}
//
QStandardItem* QMakeProjectItemModel::projectItem() const
{
	return iProject;
}
//
QStringList QMakeProjectItemModel::subProjects() const
{
	// check if project is type subdirs
	if ( getValue( "template" ).toLower() != "subdirs" )
		return QStringList();
	// get subdirs values
	QStringList lf, ls = getValuesList( "subdirs" );
	QDir d;
	// loop on all values
	for ( int i = 0; i < ls.count(); i++ )
	{
		// go to sub project folder
		d.setPath( path().append( ls.value( i ).prepend( "/" ) ) );
		// set path clean and absolute
		d.setPath( d.canonicalPath() );
		// if folder doesn't exist go next
		if ( !d.exists() )
			continue;
		// getting all pro files where name like subproject name + .pro
		QStringList ld = d.entryList( QStringList() << QString( "%1.pro" ).arg( d.dirName() ), QDir::Files | QDir::NoDotAndDotDot );
		// if found 1 or more, add the first to lf
		if ( ld.count() )
			lf << d.absoluteFilePath( ld.value( 0 ) );
	}
	return lf;
}
// recursive get in case of folder
QStringList get( QStandardItem* it )
{
	QStringList l;
	if ( !it )
		return l;
	for ( int i = 0; i < it->rowCount(); i++ )
	{
		if ( it->child( i )->rowCount() )
			l << get( it->child( i ) );
		if ( it->child( i ) ->type() != QMakeProjectItem::Folder )
			l << it->child( i )->data( QMakeProjectItem::ValueRole ).toString();
	}
	return l;
}
//
QString QMakeProjectItemModel::getValue( const QString& s ) const
{
	return getValuesList( s ).value( 0 );
}
//
QStringList QMakeProjectItemModel::getValuesList( const QString& s ) const
{
	QStringList l;
	foreach ( QStandardItem* i, itemsByName( s ) )
		l << get( i );
	return l;
}
//
QString QMakeProjectItemModel::getValues( const QString& s ) const
{
	return getValuesList( s ).join( " " );
}
//
QList<QStandardItem*> QMakeProjectItemModel::itemsByName( const QString& s ) const
{
	QList<QStandardItem*> l = findItems( s, Qt::MatchFixedString | Qt::MatchRecursive );
	foreach ( QStandardItem* i, l )
		if ( i->type() != QMakeProjectItem::Variable )
			l.removeAll( i );
	return l;
}
//
QStandardItem* QMakeProjectItemModel::itemByName( const QString& s ) const
{
	return itemsByName( s ).value( 0 );
}
//
bool QMakeProjectItemModel::open( bool )
{
	if ( mIsOpen )
		return false;
	mIsOpen = parse();
	if ( mIsOpen )
	{
		if ( getValue( "TEMPLATE" ).toLower() != "subdirs" )
			prepareCompletion();
		emit isOpenChanged( true );
	}
	return mIsOpen;
}
// preapre completion
void QMakeProjectItemModel::prepareCompletion()
{
	QStringList l, f, e = QStringList() << "*.h*" << "*.c*" ;
	QString s;
	// load qt prepared api if needed
	s = getValue( "LANGUAGE" ).toLower();
	if ( s.contains( "qt3" ) && mAPIs->isPrepared( "Qt3.api" ) )
		mAPIs->loadPrepared( "Qt3.api" );
	else if ( mAPIs->isPrepared( "Qt4.api" ) )
		mAPIs->loadPrepared( "Qt4.api" );
	// add headers to apis
	l = getValuesList( "HEADERS" );
	// add sources to apis
	l << getValuesList( "SOURCES" );
	// includepath
	f = getValuesList( "INCLUDEPATH" );
	foreach ( s, f )
	{
		s = filePath( s );
		if ( QFile::exists( s ) )
			l << getFiles( QDir( s ), e );
	}
	// dependpath
	f = getValuesList( "DEPENDPATH" );
	foreach ( s, f )
	{
		s = filePath( s );
		if ( QFile::exists( s ) )
			l << getFiles( QDir( s ), e );
	}
	// vpath
	f = getValuesList( "VPATH" );
	foreach ( s, f )
	{
		s = filePath( s );
		if ( QFile::exists( s ) )
			l << getFiles( QDir( s ), e );
	}
	// load files
	f.clear();
	foreach ( s, l )
	{
		s = filePath( s );
		if ( !f.contains( s ) )
		{
			f << s;
			if ( QFile::exists( s ) )
				mAPIs->load( s );
		}
	}
	// start prepare thread
	mAPIs->prepare();
}
// parse project
bool QMakeProjectItemModel::parse()
{
	// check time to parse a project
	QTime mTime;
	mTime.start();
	// open project file
	QFile f( mFilePath );
	if ( !f.exists() || !f.open( QFile::ReadOnly | QFile::Text ) )
	{
		qWarning( qPrintable( tr( "Can't open project: %1" ).arg( mFilePath ) ) );
		return false;
	}
	// create project item
	iProject = new QMakeProjectItem( QMakeProjectItem::Project );
	iProject->setData( filePath(), QMakeProjectItem::AbsoluteFilePathRole );
	iProject->setText( name() );
	setItemIcon( iProject );
	appendRow( iProject );
	setHorizontalHeaderItem( 0, iProject->clone() );
	// look for codec
	QString mCodec = "System";
	QString mBuffer = f.readAll();
	int i = mBuffer.indexOf( "CODEC\t=" );
	if ( i != -1 )
	{
		i += 7;
		mCodec = mBuffer.mid( i, mBuffer.indexOf( "\n", i ) -i );
		mCodec = mCodec.left( mBuffer.indexOf( " " ) ).trimmed();
	}
	// create textstream with correct codec
	QTextStream ts( &f );
	ts.setCodec( QTextCodec::codecForName( qPrintable( mCodec ) ) );
	ts.seek( 0 );
	//
	QString mLine, mTemp;
	int mBrace1 = 0, mBrace2 = 0;
	bool isMultiLine = false, isAtMultiLineEnd = false;
	// parse project
	mBuffer.clear();
	while ( !ts.atEnd() )
	{
		mLine = ts.readLine();
		// counting brace to know if we are in a block
		mBrace1 += mLine.count( '{' );
		mBrace1 -= mLine.count( '}' );
		mBrace2 += mLine.count( '(' );
		mBrace2 -= mLine.count( ')' );
		// check line
		//if ( !mLine.isEmpty() )
		{
			mTemp = mLine;
			// check is line contains a comment
			i = mTemp.lastIndexOf( '#' );			
			if ( i != -1 )
			{
				// remove temp comment
				mTemp.truncate( i );
				// trim string
				mTemp = mTemp.trimmed();
			}
			// check if line is multiline
			if ( !isMultiLine && mTemp.endsWith( '\\' ) )
				isMultiLine = true;
			else if ( isMultiLine && !mTemp.endsWith( '\\' ) )
				isAtMultiLineEnd = true;
			// add to buffer to parse
			mBuffer += mLine.trimmed() +"\n";
		}
		// if mBrace1 = 0 and mBrace2 = 0 we can parse this part
		if ( !mBrace1 && // closed { } block
			!mBrace2 && // closed ( ) block
			!mBuffer.isEmpty() && // not empty buffer
			( !isMultiLine || ( isMultiLine && isAtMultiLineEnd ) ) )
		{
			// parse line or multiline
			if ( mBuffer.count( '{' ) == 0 )
				parseLine( mBuffer, iProject );
			// parse block, muffer contains {
			else
				parseBlock( mBuffer, iProject );
			// clear buffer
			mBuffer.clear();
			// reset multiline state
			isMultiLine = false;
			isAtMultiLineEnd = false;
		}
	}
	// show time elapsed
	//warn( tr( "Parse Time: %1" ).arg( QString::number( float( mTime.elapsed() ) / 1000 ) ) );
	// need check brace count to know if there was an error or not at parsing
	return true;
}
// append item to iparent
void QMakeProjectItemModel::appendRow( QStandardItem* item, QStandardItem* iParent )
{
	if ( !iParent )
		AbstractProjectItemModel::appendRow( item );
	else
		iParent->appendRow( item );
}
// return the parameters string of a function as a qstringlist
QStringList QMakeProjectItemModel::parseFunctionParameters( const QString& s )
{
	QStringList l;
	QString mTemp;
	int i = 0;
	rx.setPattern( "\"(\\\\\"|\\\\\\\\|[^\"])*\"|([\\w/\\.\\+\\*\\$\\[\\]\\(\\)]+)" );
	while ( ( i = rx.indexIn( s, i ) ) != -1 )
	{
		mTemp = s.mid( i, rx.matchedLength() );
		if ( mTemp.startsWith( '"' ) && mTemp.endsWith( '"' ) )
		{
			mTemp.chop( 1 );
			mTemp.remove( 0, 1 );
		}
		l << mTemp;
		i += rx.matchedLength();
	}
	return l;
}
// parse a single line
void QMakeProjectItemModel::parseLine( const QString& line, QMakeProjectItem* item )
{
	QString mLine( line.trimmed() );
	// empty line
	if ( mLine.isEmpty() )
	{
		// create item as EmptyLine type
		QMakeProjectItem* iEmptyLine = new QMakeProjectItem( QMakeProjectItem::EmptyLine );
		iEmptyLine->setText( tr( "Empty Line" ) );
		iEmptyLine->setToolTip( tr( "Contents: Empty Line" ) );
		setItemIcon( iEmptyLine );
		appendRow( iEmptyLine, item );
		return;
	}
	// comment
	else if ( mLine.startsWith( "#" ) )
	{
		QMakeProjectItem* iComment = new QMakeProjectItem( QMakeProjectItem::Comment );
		iComment->setText( mLine );
		iComment->setToolTip( tr( "Comment: %1" ).arg( mLine ) );
		setItemIcon( iComment );
		appendRow( iComment, item );
		return;
	}
	//
	rx.setPattern( "([^\\s\\w\\.!\\$]+)" );
	int i = 0;
	QString mOperator, mKeyword, mTemp;
	QMakeProjectItem* iLast = item;
	while ( ( i = rx.indexIn( mLine, i ) ) != -1 )
	{
		mOperator = mLine.mid( i, rx.matchedLength() );
		mKeyword = mLine.left( i ).trimmed();
		mLine = mLine.mid( i +rx.matchedLength() ).trimmed();
		i = 0;
		//
		if ( mOperator == ":" )
		{
			QMakeProjectItem* iScope = new QMakeProjectItem( QMakeProjectItem::Scope );
			iScope->setText( mKeyword );
			iScope->setData( mKeyword.startsWith( "!" ), QMakeProjectItem::NegateRole );
			iScope->setToolTip( tr( "Scope: %1" ).arg( mKeyword ) );
			setItemIcon( iScope );
			appendRow( iScope, iLast );
			iLast = iScope;
		}
		else if ( mOperator.contains( "=" ) )
		{
			// create item
			QMakeProjectItem* iVariable = new QMakeProjectItem( QMakeProjectItem::Variable );
			iVariable->setText( mKeyword );
			iVariable->setData( mOperator, QMakeProjectItem::OperatorRole );
			iVariable->setToolTip( tr( "Operator: %1\nContents:\n%2" ).arg( mOperator, mLine ) );
			setItemIcon( iVariable );
			appendRow( iVariable, iLast );
			// now need to parse content and create items contents
			// split entries by \n
			QStringList l = mLine.split( "\n" );
			QString mComment;
			rx.setPattern( "\"(?:\\\\\"|\\\\\\\\|[^\"])+\"|(?:[\\w/\\.\\+\\*\\$\\[\\]\\(\\)]+)" );
			QMakeProjectItem* iValue = 0;
			foreach ( QString s, l )
			{
				// got comment if it exists
				if ( ( i = s.lastIndexOf( '#' ) ) != -1 )
				{
					mComment = s.mid( i );
					s = s.mid( 0, i ).trimmed();
				}
				i = 0;
				while ( ( i = rx.indexIn( s, i ) ) != -1 )
				{
					// got value
					mTemp = s.mid( i, rx.matchedLength() );
					// increment i by matchedlength
					i += rx.matchedLength();
					// remove # at end and starts if exists
					if ( mTemp.startsWith( '"' ) && mTemp.endsWith( '"' ) )
					{
						mTemp.chop( 1 );
						mTemp.remove( 0, 1 );
					}
					//
					if ( simpleModelVariables().contains( iVariable->text(), Qt::CaseInsensitive ) )
					{
						iValue = new QMakeProjectItem( QMakeProjectItem::File );
						iValue->setData( mTemp, QMakeProjectItem::ValueRole );
						iValue->setData( filePath( mTemp ), QMakeProjectItem::AbsoluteFilePathRole );
						iValue->setToolTip( tr( "Value: %1" ).arg( mTemp ) );
						setItemIcon( iValue );
						//
						QFileInfo mFile( mTemp );
						iValue->setText( mFile.fileName() );
						appendRow( iValue, getFolder( mFile.path(), iVariable ) );
					}
					else
					{
						iValue = new QMakeProjectItem( QMakeProjectItem::Value );
						iValue->setData( mTemp, QMakeProjectItem::ValueRole );
						iValue->setToolTip( tr( "Value: %1" ).arg( mTemp ) );
						setItemIcon( iValue );
						//
						iValue->setText( mTemp );
						appendRow( iValue, iVariable );
					}
				}
				// add comment to last entry
				if ( iValue && !mComment.isEmpty() )
				{
					iValue->setData( mComment, QMakeProjectItem::CommentRole );
					iValue->setToolTip( tr( "%1Comment: %2" ).arg( iValue->toolTip().append( "\n" ), mComment ) );
					mComment.clear();
				}
			}
			// return
			return;
		}
		else if ( mOperator == "(" )
		{
			QString mComment;
			// got comment if it exists
			if ( ( i = mLine.lastIndexOf( '#' ) ) != -1 )
			{
				mComment = mLine.mid( i );
				mLine = mLine.mid( 0, i );
			}
			mLine = mLine.left( mLine.lastIndexOf( ")" ) ).trimmed();
			//
			QStringList mParameters = parseFunctionParameters( mLine );
			QMakeProjectItem* iFunction = new QMakeProjectItem( QMakeProjectItem::Function );
			iFunction->setText( mKeyword );
			iFunction->setData( mKeyword.startsWith( "!" ), QMakeProjectItem::NegateRole );
			iFunction->setData( mParameters, QMakeProjectItem::ParametersRole );
			iFunction->setData( mComment, QMakeProjectItem::CommentRole );
			iFunction->setToolTip( tr( "Function: %1\nParameters:\n%2\nComment: %3" ).arg( mKeyword, mParameters.join( "\n" ), mComment ) );
			setItemIcon( iFunction );
			appendRow( iFunction, iLast );
			//
			return;
		}
	}
}
// this parse a bloc ie: { } or ( )
void QMakeProjectItemModel::parseBlock( const QString& mBlock, QMakeProjectItem* )
{
	return;
	//warn( "parseBlock: " +mBlock );
	rx.setPattern( "([^\\s\\w]+)" );
	int i = 0;
	QString s;
	int mBrace1 = 0, mBrace2 = 0, iStart = 0, iEnd = 0;
	bool InFunction = false;
	while ( ( i = rx.indexIn( mBlock, i ) ) != -1 )
	{
		s = mBlock.mid( i, rx.matchedLength() );
		// counting braces
		mBrace1 += s.count( '(' );
		mBrace1 -= s.count( ')' );
		mBrace2 += s.count( '{' );
		mBrace2 -= s.count( '}' );
		// starting function
		if ( s == "(" && mBrace1 == 1 )
		{
			InFunction = true;
			iStart = i +1;
		} // ending function
		else if ( s == ")" && !mBrace1 )
		{
			InFunction = false;
			iEnd = i;
			QStringList l = parseFunctionParameters( mBlock.mid( iStart, iEnd -iStart ) );
		}
		//
		i += rx.matchedLength();
	}
}
//
void QMakeProjectItemModel::setItemIcon( QMakeProjectItem* item )
{
	QString mText = item->text().toLower();
	QString mFileName;
	switch ( item->type() )
	{
	case QMakeProjectItem::Project:
		mFileName = ":/icons/icons/project.png";
		break;
	case QMakeProjectItem::Variable:
		mFileName = ":/icons/icons/variable.png";
		if ( mText == "codec" )
			mFileName = ":/icons/icons/codec.png";
		else if ( mText == "template" )
			mFileName = ":/icons/icons/template.png";
		else if ( mText == "language" )
			mFileName = ":/icons/icons/language.png";
		else if ( mText == "forms" || mText == "forms3" )
			mFileName = ":/icons/icons/forms.png";
		else if ( mText == "headers" )
			mFileName = ":/icons/icons/headers.png";
		else if ( mText == "sources" )
			mFileName = ":/icons/icons/sources.png";
		else if ( mText == "translations" )
			mFileName = ":/icons/icons/translations.png";
		else if ( mText == "resources" )
			mFileName = ":/icons/icons/resources.png";
		else if ( mText == "openedfiles" )
			mFileName = ":/icons/icons/openedfiles.png";
		break;
	case QMakeProjectItem::Value:
		mFileName = ":/icons/icons/value.png";
		break;
	case QMakeProjectItem::Scope:
		mFileName = ":/icons/icons/scope.png";
		if ( mText == "unix" )
			mFileName = ":/icons/icons/unix.png";
		else if ( mText == "mac" )
			mFileName = ":/icons/icons/mac.png";
		else if ( mText == "win32" )
			mFileName = ":/icons/icons/windows.png";
		break;
	case QMakeProjectItem::Function:
		mFileName = ":/icons/icons/function.png";
		break;
	case QMakeProjectItem::EmptyLine:
		mFileName = ":/icons/icons/emptyline.png";
		break;
	case QMakeProjectItem::Comment:
		mFileName = ":/icons/icons/comment.png";
		break;
	case QMakeProjectItem::Folder:
		mFileName = ":/icons/icons/folder.png";
		break;
	case QMakeProjectItem::File:
		mFileName = ":/icons/icons/file.png";
		break;
	default:
		mFileName = ":/icons/icons/unknow.png";
		break;
	}
	item->setIcon( QIcon( mFileName ) );
}
// return an item for mPath folder and parent item iParent
QMakeProjectItem* QMakeProjectItemModel::getFolder( const QString& mPath, QMakeProjectItem* iParent )
{
	if ( mPath.isEmpty() || mPath == "." || mPath == "./" )
		return iParent;
	//
	QMakeProjectItem* iFolder = 0;
	QMakeProjectItem* it = 0;
	// ther can be bug is path start with / need rewrite
	QStringList l = mPath.split( "/", QString::SkipEmptyParts );
	QStringList mName;
	foreach ( QString s, l )
	{
		iFolder = 0;
		mName << s;
		for ( int j = 0; j < iParent->rowCount(); j++ )
		{
			// got item
			it = (QMakeProjectItem*)iParent->child( j );
			// check if itel is the one require
			if ( it->type() == QMakeProjectItem::Folder && it->data( QMakeProjectItem::FolderPathRole ).toString() == mName.join( "/" ) )
			{
				iFolder = it;
				break;
			}
		}
		// create item if it not exists
		if ( !iFolder )
		{
			iFolder = new QMakeProjectItem( QMakeProjectItem::Folder );
			iFolder->setText( s );
			iFolder->setToolTip( tr( "Path: %1" ).arg( mName.join( "/" ) ) );
			iFolder->setData( mName.join( "/" ), QMakeProjectItem::FolderPathRole );
			setItemIcon( iFolder );
			iParent->insertRow( 0, iFolder );
		}
		// set parent now this new item
		iParent = iFolder;
	}
	//
	return iFolder;
}
//
void QMakeProjectItemModel::close()
{
	if ( !mIsOpen )
		return;
	if ( !mIsModified )
		save();
	clear();
	mIsOpen = false;
	emit isOpenChanged( false );
}
//
void QMakeProjectItemModel::save()
{
	if ( !mIsOpen || !mIsModified )
		return;
	// need do save stuff here
	mIsModified = false;
	emit isModifiedChanged( false );
}
