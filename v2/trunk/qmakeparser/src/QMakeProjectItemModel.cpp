#include "QMakeProjectItemModel.h"
#include "QMakeProjectItem.h"
//
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegExp>
//
static QRegExp rx;
//
void warn( const QString& s )
{
	qWarning( qPrintable( s ) );
}
//
QString quoted( const QString& s )
{
	return QString( "'%1'" ).arg( s );
}
//
QMakeProjectItemModel::QMakeProjectItemModel( const QString& s, QObject* p )
	: AbstractProjectItemModel( s, p )
{
}
//
bool QMakeProjectItemModel::openProject( bool )
{
	if ( mIsOpen )
		return false;
	mIsOpen = parseProject();
	if ( mIsOpen )
		emit projectOpened( true );
	return mIsOpen;
}
// parse project
bool QMakeProjectItemModel::parseProject()
{
	// check time to parse a project
	QTime mTime;
	mTime.start();
	// open project file
	QFile f( mFilePath );
	if ( !f.exists() || !f.open( QFile::ReadOnly | QFile::Text ) )
	{
		warn( QString( "Can't open project: %s" ).arg( mFilePath ) );
		return false;
	}
	// create project item
	QMakeProjectItem* iProject = new QMakeProjectItem( QMakeProjectItem::Project );
	iProject->setData( filePath(), QMakeProjectItem::AbsoluteFilePathRole );
	iProject->setText( name() );
	setItemIcon( iProject );
	appendRow( iProject );
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
	warn( tr( "Parse Time: %1" ).arg( QString::number( float( mTime.elapsed() ) / 1000 ) ) );
	// need check brace count to know if there was an error or not at parsing
	return true;
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
		// create item as Empty type
		QMakeProjectItem* iEmptyLine = new QMakeProjectItem( QMakeProjectItem::EmptyLine );
		iEmptyLine->setText( tr( "Empty Line" ) );
		iEmptyLine->setToolTip( tr( "Contents: Empty Line" ) );
		setItemIcon( iEmptyLine );
		item->appendRow( iEmptyLine );
		return;
	}
	// comment
	else if ( mLine.startsWith( "#" ) )
	{
		QMakeProjectItem* iComment = new QMakeProjectItem( QMakeProjectItem::Comment );
		iComment->setText( mLine );
		iComment->setToolTip( tr( "Comment: %1" ).arg( mLine ) );
		setItemIcon( iComment );
		item->appendRow( iComment );
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
			iLast->appendRow( iScope );
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
			iLast->appendRow( iVariable );
			iLast = iVariable;
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
					iValue = new QMakeProjectItem( QMakeProjectItem::Value );
					mTemp = s.mid( i, rx.matchedLength() );
					if ( mTemp.startsWith( '"' ) && mTemp.endsWith( '"' ) )
					{
						mTemp.chop( 1 );
						mTemp.remove( 0, 1 );
					}
					iValue->setText( mTemp );
					setItemIcon( iValue );
					//
					iLast->appendRow( iValue );
					i += rx.matchedLength();
				}
				// add comment to last entry
				if ( iValue && !mComment.isEmpty() )
				{
					iValue->setData( mComment, QMakeProjectItem::CommentRole );
					iValue->setToolTip( tr( "Comment: %1" ).arg( mComment ) );
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
			iLast->appendRow( iFunction );
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
	qWarning( qPrintable( item->data().toString() ) );
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
//
void QMakeProjectItemModel::closeProject()
{
	clear();
	mIsOpen = false;
	emit projectOpened( false );
}
