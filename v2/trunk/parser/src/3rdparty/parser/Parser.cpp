#include "Parser.h"
#include "ProjectItem.h"

#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <iostream.h>

Parser::Parser() : QObject()
{
	indent = 0;
}
//
bool Parser::parseFile(const QString& filePath)
{
	QFile f( filePath );
	if ( !f.exists() || !f.open( QFile::ReadOnly | QFile::Text ) )
	{
		qWarning( qPrintable( tr( "Can't open project: %1" ).arg( filePath ) ) );
		return false;
	}
	QTextStream ts( &f );
	parseText(ts);
	return true;
}
//
void Parser::parseText(QTextStream& ts, ProjectItem* parent)
{
	QString mBuffer;
	int i;
	//
	QString mLine, mTemp;
	int mBrace1 = 0, mBrace2 = 0;
	bool isMultiLine = false, isAtMultiLineEnd = false;
	// parse project
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
				parseLine( mBuffer, parent );
			// parse block, muffer contains {
			else
				parseBlock( mBuffer, parent );
			// clear buffer
			mBuffer.clear();
			// reset multiline state
			isMultiLine = false;
			isAtMultiLineEnd = false;
		}
	}
}
//
void Parser::parseLine(const QString& line, ProjectItem* parent)
{
	//qWarning (qPrintable("XXX"+line.trimmed()+"XXX"+QString::number((uint)parent)));
	ProjectItem* item;
	QString mLine( line.trimmed() );
	//empty && comment
	if ( mLine.isEmpty() || mLine.startsWith( "#" ))
	{
		item = new ProjectItem(ProjectItem::Line);
		item->setValue(ProjectItem::ValueRole, mLine);
		item->setValue(ProjectItem::IndentRole, indent);
		appendItem(item, parent);
		return;
	}
	else
	{
		QRegExp rx;
		rx.setPattern("(\\:|\\(|\\+?\\-?\\=)");
		int i = 0;
		QString mOperator, mKeyword, mTemp;
		if ( ( i = rx.indexIn( mLine, i ) ) != -1 )
		{
			mOperator = mLine.mid( i, rx.matchedLength() );
			mKeyword = mLine.left( i ).trimmed();
			mLine = mLine.mid( i +rx.matchedLength() ).trimmed();
			i = 0;
			if ( mOperator == ":" )
			{
				item = new ProjectItem( ProjectItem::Variable );
				item->setValue(ProjectItem::ValueRole, mKeyword);
				item->setValue(ProjectItem::OperatorRole, mOperator);
				item->setValue(ProjectItem::IndentRole, indent);

				parseLine(mLine, item);
				//ProjectItem* iValue = new ProjectItem( ProjectItem::Value );
				//iValue->setValue( ProjectItem::ValueRole, mLine);
				//item->setValue(ProjectItem::ChildRole, (uint)iValue);
				appendItem(item, parent);
				return;
			}
			else if ( mOperator.contains( "=" ) )
			{
				item = new ProjectItem(ProjectItem::Variable);
				item->setValue(ProjectItem::ValueRole, mKeyword);
				item->setValue(ProjectItem::OperatorRole, mOperator);
				if (!parent || parent->value(ProjectItem::OperatorRole) != ":" )
					item->setValue(ProjectItem::IndentRole, indent);
				QStringList l = mLine.split( "\n" );
				QString mComment;
				ProjectItem* iValue = 0;
				int counter = 0;
				foreach ( QString s, l )
				{
					++counter;
					// got comment if it exists
					if ( ( i = s.lastIndexOf( '#' ) ) != -1 )
					{
						mComment = s.mid( i );
						s = s.mid( 0, i ).trimmed();
					}
					//
					iValue = new ProjectItem( ProjectItem::Value );
					iValue->setValue( ProjectItem::ValueRole, s);
					if (counter>1)
						iValue->setValue(ProjectItem::IndentRole, item->value(ProjectItem::IndentRole).toInt()+1);
					item->setValue(ProjectItem::ChildRole, (uint)iValue);
					//
					// add comment to last entry
					if ( iValue && !mComment.isEmpty() )
					{
						iValue->setValue( ProjectItem::CommentRole, mComment);
						mComment.clear();
					}
				}
				appendItem(item, parent);
				return;
			}
			else if ( mOperator == "(" )
			{
				//qWarning(qPrintable(mLine));
				QString mComment;
				// got comment if it exists
				if ( ( i = mLine.lastIndexOf( '#' ) ) != -1 )
				{
					mComment = mLine.mid( i );
					mLine = mLine.mid( 0, i );
				}
				int close = mLine.lastIndexOf( ")" );
				QString tmpLine = mLine.left( close ).trimmed();
				mKeyword = mKeyword+QString("(%1)").arg(tmpLine);
				//qWarning(qPrintable(mLine));
				mLine = mLine.mid( close+1, i ).trimmed();
				//qWarning(qPrintable(mLine));
				//
				//qWarning(qPrintable(QString::number(close)+" "+QString::number(i)+" "+mLine));
				if (mLine.startsWith(":"))
				{
					item = new ProjectItem( ProjectItem::Variable );
					item ->setValue( ProjectItem::OperatorRole, ":" );

					parseLine(mLine.mid(1, i), item);
					//ProjectItem* iValue = new ProjectItem( ProjectItem::Value );
					//iValue->setValue( ProjectItem::ValueRole, mLine.mid(1, i));

					//item->setValue(ProjectItem::ChildRole, (uint)iValue);
				}
				else
				{
					item = new ProjectItem( ProjectItem::Value );
				}
				item->setValue( ProjectItem::ValueRole, mKeyword );
				item->setValue( ProjectItem::CommentRole, mComment );
				item->setValue(ProjectItem::IndentRole, indent);
				//
				appendItem(item, parent);
				return;
			}
		}
	}
}
//
void Parser::parseBlock(const QString& block, ProjectItem* parent)
{
	if (block.isEmpty())
		return;
	int opened1 = block.indexOf("{");
	int opened2 = block.indexOf("{", opened1+1);
	int closed = block.indexOf("}", opened1+1);

	if (closed != -1 && (closed<opened2 || (opened2 == -1 && opened1 != -1)))
	{
		parseSimpleBlock(block.left(closed+1), parent);
		parseBlock(block.mid(closed+1).trimmed(), parent);
	}
	else if (opened2 != -1 && closed>opened2 )
	{
		ProjectItem* item = new ProjectItem(ProjectItem::Function);
		QString text = checkForComment(block.left(opened1).trimmed(), parent);
		item->setValue(ProjectItem::ValueRole, text);
		item->setValue(ProjectItem::IndentRole, indent);
		complexScopeList<<item;

		appendItem(item, parent);
		++indent;
		parseBlock(block.mid(opened1+1).trimmed(), item);
	}
	else if (!closed && opened1 == -1 && opened2 == -1)
	{
		complexScopeList.removeLast();
		ProjectItem* iEnd = new ProjectItem(ProjectItem::FunctionEnd);
		--indent;
		iEnd->setValue(ProjectItem::IndentRole, indent);

		if (!complexScopeList.isEmpty())
		{
			parent = complexScopeList.last();
			parent->setValue(ProjectItem::ChildRole, (uint)iEnd);
		}
		else
		{
			parent = 0;
			mProjectItemsList<<iEnd;
		}
		parseBlock(block.mid(closed+1).trimmed(), parent);
	}
}
//
void Parser::parseSimpleBlock(const QString& block, ProjectItem* parent)
{
	QRegExp rx;
	rx.setPattern("^([^\{]+)\\{([^\}]+)");
	int pos = rx.indexIn(block);
	if ( pos != -1)
	{
		ProjectItem* item = new ProjectItem(ProjectItem::Function);
		QString text = checkForComment(rx.cap(1).trimmed(), parent);
		item->setValue(ProjectItem::ValueRole, text);
		item->setValue(ProjectItem::IndentRole, indent);

		appendItem(item, parent);
		QTextStream ts(rx.cap(2).trimmed().toLocal8Bit());
		++indent;
		parseText(ts, item);
		--indent;
		ProjectItem* iEnd = new ProjectItem(ProjectItem::FunctionEnd);
		iEnd->setValue(ProjectItem::IndentRole, indent);
		item->setValue(ProjectItem::ChildRole, (uint)iEnd);
	}
}
//
void Parser::writeToBuffer(QList<ProjectItem*> childList)
{
	QList<ProjectItem*> list;
	if (!childList.isEmpty())
		list = childList;
	else
		list = mProjectItemsList;
	QList<ProjectItem*>::iterator it = list.begin();
	while (it != list.end())
	{
		bool hasChild = (*it)->hasChilds();
		QString line;
		QString tab = QString().fill('\t',(*it)->value(ProjectItem::IndentRole).toInt());
		switch ((*it)->type())
		{
			case ProjectItem::Line:
				line = tab+(*it)->value(ProjectItem::ValueRole).toString()+"\n";
				break;
			case ProjectItem::Variable:
				line = tab+(*it)->value(ProjectItem::ValueRole).toString()+
					   QString(" %1 ").arg((*it)->value(ProjectItem::OperatorRole).toString());
				break;
			case ProjectItem::Value:
				line = tab+(*it)->value(ProjectItem::ValueRole).toString()+
					   QString("\t%1\n").arg((*it)->value(ProjectItem::CommentRole).toString());
				break;
			case ProjectItem::Function:
				line = tab+(*it)->value(ProjectItem::ValueRole).toString()+" {\n";
				break;
			case ProjectItem::FunctionEnd:
				line = tab+"}\n";
				break;
		}
		// make "} \n else" to "} else"
		if (line.indexOf(QRegExp("^(\\s+)?\\belse\\b(\\s+)?(\\:)?")) != -1 && mFileText.endsWith("}\n"))
		{
			line.remove(QRegExp("^(\\s+)"));
			mFileText.chop(1);
			mFileText+=" ";
		}
		//
		mFileText.append(line);
		if (hasChild)
		{
			writeToBuffer((*it)->childs());
		}
		++it;
	}
}
//
void Parser::writeToFile()
{
	cout <<mFileText.toStdString()<<endl;
	mFileText.clear();
	indent = 0;
}
//
QString Parser::checkForComment(QString text, ProjectItem* parent)
{
	QString returnString;
	QStringList list = text.split("\n", QString::SkipEmptyParts);
	QStringList::iterator it = list.begin();
	while (it != list.end())
	{
		if ((*it).trimmed().startsWith("#"))
			parseLine(*it, parent);
		else
			returnString +=QString("\n%1").arg(*it);
		++it;
	}
	return returnString.trimmed();
}
//
void Parser::appendItem(ProjectItem* item, ProjectItem* parent)
{
	if (parent)
		parent->setValue(ProjectItem::ChildRole, (uint)item);
	else
		mProjectItemsList<<item;
}