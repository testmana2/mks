#include "ProjectItemModel.h"
#include "ProjectItem.h"

#include <QFile>
#include <QRegExp>
#include <QFileInfo>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <iostream.h>

ProjectItemModel::ProjectItemModel() : QStandardItemModel()
{
	indent = 0;
	mProject = 0;
}
//
HashSimpleSettings ProjectItemModel::initializeSimpleSettings()
{
	HashSimpleSettings mSimpleSettings;
	mSimpleSettings["CONFIG"].insert(QString(), 0);
	mSimpleSettings["QT"].insert(QString(), 0);
	mSimpleSettings["TEMPLATE"].insert(QString(), 0);
	mSimpleSettings["TARGET"].insert(QString(), 0);
	mSimpleSettings["LANGUAGE"].insert(QString(), 0);
	mSimpleSettings["OPTIMIZATION"].insert(QString(), 0);
	mSimpleSettings["AUTHOR"].insert(QString(), 0);
	mSimpleSettings["VERSION"].insert(QString(), 0);
	mSimpleSettings["DESTDIR"].insert(QString(), 0);
	mSimpleSettings["SUBDIRS"].insert(QString(), 0);
	mSimpleSettings["LIBS"].insert(QString(), 0);
	mSimpleSettings["INCLUDEPATH"].insert(QString(), 0);
	mSimpleSettings["TRANSLATIONS"].insert(QString(), 0);
	mSimpleSettings["RESOURCES"].insert(QString(), 0);
	mSimpleSettings["DEPENDPATH"].insert(QString(), 0);
	mSimpleSettings["DEF_FILE"].insert(QString(), 0);
	mSimpleSettings["RC_FILE"].insert(QString(), 0);
	mSimpleSettings["RES_FILE"].insert(QString(), 0);
	return mSimpleSettings;
}
//
HashSimpleSettings ProjectItemModel::simpleSettings( const QModelIndex& index )
{
	return 	mProjectList[(uint)currentProject(index)];
}
// pasnox
void ProjectItemModel::updateItem( ProjectItem* i )
{
	if ( i )
		setItemData( i );
}
//
bool ProjectItemModel::openFile(QString& filePath)
{
	if (filePath.isEmpty() || !QFileInfo(filePath).exists())
	{
		filePath = QFileDialog::getOpenFileName(0, "Choose project",	"/home", "Projects (*.pro)");
	}
	bool b = parseFile(filePath);
	if (b)
	{
		mProject = (ProjectItem*)horizontalHeaderItem(0);
		return true;
	}
	return false;
}
//
bool ProjectItemModel::parseFile(const QString& filePath, ProjectItem* parent)
{
	QFile f( filePath );
	if ( !f.exists() || !f.open( QFile::ReadOnly | QFile::Text ) )
	{
		qWarning( qPrintable( tr( "Can't open project: %1" ).arg( filePath ) ) );
		return false;
	}
	QTextStream ts( &f );
	ProjectItem* iProject = createProjectItem(QFileInfo(filePath).absoluteFilePath(), parent);

	parseText(ts, parent?iProject:0);

	foreach(QString subProjectPath, absoluteSubProjectsPath(iProject))
	{
		parseFile(subProjectPath, iProject);
	}
	return true;
}
//
QStringList ProjectItemModel::absoluteSubProjectsPath( ProjectItem* parentProject )
{
	// get subdirs values
	ProjectItem* tmp = 0;
	if (parentProject != horizontalHeaderItem(0) )
		tmp = parentProject;
	QStringList lf, ls = getValuesList( "SUBDIRS" ,QStringList());
	QDir d;
	// loop all subdirs
	foreach ( QString s, ls )
	{
		// go to subdir folder
		QString parentPath = QFileInfo(parentProject->data(ProjectItem::AbsoluteFilePathRole).toString()).absolutePath();
		d.setPath( parentPath.append( s.prepend( "/" ) ) );
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
//
ProjectItem* ProjectItemModel::createProjectItem(const QString& filePath, ProjectItem* parent)
{
	ProjectItem* iProject = new ProjectItem( ProjectItem::Project );
	iProject->setData( filePath, ProjectItem::AbsoluteFilePathRole );
	setItemData(iProject);
	mProject = iProject;
	if (!parent)
	{
		setHorizontalHeaderItem( 0, iProject);
		iProject->setTextAlignment(Qt::AlignLeft);
	}
	else if (parent == horizontalHeaderItem(0))
	{
		appendItem(iProject, 0);
	}
	else
		appendItem(iProject, parent);

	mProjectList.insert((uint)iProject, initializeSimpleSettings());

	return iProject;
}
//
void ProjectItemModel::parseText(QTextStream& ts, ProjectItem* parent)
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
QStringList ProjectItemModel::fileNameBasedVariables()
{
	return QStringList() << "FORMS" << "FORMS3" << "HEADERS" << "SOURCES"
		   << "TRANSLATIONS" << "RESOURCES" << "OPENEDFILES"
		   << "DEF_FILE" << "RC_FILE" << "RES_FILE" << "YACCSOURCES";
}
//
void ProjectItemModel::parseLine(const QString& line, ProjectItem* parent)
{
	ProjectItem* item;
	QString mLine( line.trimmed() );
	//empty && comment
	if ( mLine.isEmpty() || mLine.startsWith( "#" ))
	{
		item = new ProjectItem(ProjectItem::Line);
		item->setData(mLine, ProjectItem::ValueRole);
		item->setData( indent, ProjectItem::IndentRole);
		appendItem(item, parent);
		return;
	}
	else
	{
		QRegExp rx;
		rx.setPattern("(\\:|\\(|\\+?\\-?\\~?\\*?\\=)");
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
				item->setData(mKeyword, ProjectItem::ValueRole );
				item->setData(mOperator, ProjectItem::OperatorRole );
				item->setData(indent, ProjectItem::IndentRole );

				parseLine(mLine, item);

				appendItem(item, parent);
				return;
			}
			else if ( mOperator.contains( "=" ) )
			{
				item = new ProjectItem(ProjectItem::Variable);
				item->setData(mKeyword, ProjectItem::ValueRole);
				item->setData(mOperator, ProjectItem::OperatorRole);
				if (!parent || parent->data(ProjectItem::OperatorRole) != ":" )
					item->setData(indent, ProjectItem::IndentRole);
				//Simple settings
				if (!parent || parent->type() == ProjectItem::Project)
				{
					ProjectItem* tmpParent = parent;
					if (!tmpParent)
						tmpParent = (ProjectItem*)horizontalHeaderItem(0);

					HashSimpleSettings::iterator it = mProjectList[(uint)tmpParent].find(mKeyword);
					if( it!= mProjectList[(uint)tmpParent].end())
					{
						it.value().insertMulti(mOperator, (uint)item);
					}
				}
				//
				QStringList l = mLine.split( "\n" );
				QString mComment;
				ProjectItem* iValue = 0;
				foreach ( QString s, l )
				{
					// got comment if it exists
					if ( ( i = s.lastIndexOf( '#' ) ) != -1 )
					{
						mComment = s.mid( i );
						s = s.mid( 0, i ).trimmed();
					}
					//"FORMS" "HEADERS" "SOURCES"...
					if ( fileNameBasedVariables().contains( mKeyword, Qt::CaseInsensitive ) )
					{
						rx.setPattern( "\"(?:\\\\\"|\\\\\\\\|[^\"])+\"|(?:[\\w/\\.\\+\\*\\$\\[\\]\\(\\)]+)" );
						i = 0;
						while ( ( i = rx.indexIn( s, i ) ) != -1 )
						{
							mTemp = s.mid( i, rx.matchedLength() );
							// increment i by matchedlength
							i += rx.matchedLength();
							// remove # at end and starts if exists
							if ( mTemp.startsWith( '"' ) && mTemp.endsWith( '"' ) )
							{
								mTemp.chop( 1 );
								mTemp.remove( 0, 1 );
							}
							iValue = new ProjectItem( ProjectItem::File );
							iValue->setData( /*QFileInfo(*/mTemp/*).fileName()*/, ProjectItem::ValueRole );
							//
							QString filePath = QFileInfo(mProject->data(ProjectItem::AbsoluteFilePathRole).toString()).absolutePath();
							filePath+=QString("/%1").arg(mTemp);
							iValue->setData( filePath, ProjectItem::AbsoluteFilePathRole );
							iValue->setData(item->data(ProjectItem::IndentRole).toInt()+1, ProjectItem::IndentRole);

							appendItem(iValue, getFolder( QFileInfo(mTemp).path(), item));
						}
						//
					}
					else
					{
						s.remove(QRegExp("\\s+\\\\$"));
						iValue = new ProjectItem( ProjectItem::Value );
						iValue->setData( s, ProjectItem::ValueRole );
						iValue->setData(item->data(ProjectItem::IndentRole).toInt()+1, ProjectItem::IndentRole);
						appendItem(iValue, item);
					}
					//
					// add comment to last entry
					if ( iValue && !mComment.isEmpty() )
					{
						iValue->setData( mComment, ProjectItem::CommentRole );
						mComment.clear();
					}
				}
				if ( fileNameBasedVariables().contains( mKeyword, Qt::CaseInsensitive ) )
					item->sortChildren(0);
				appendItem(item, parent);
				return;
			}
			else if ( mOperator == "(" )
			{
				QString mComment;
				QString tmpLine = mLine;
				// got comment if it exists
				if ( ( i = mLine.lastIndexOf( '#' ) ) != -1 )
				{
					mComment = mLine.mid( i );
					tmpLine = mLine.mid( 0, i );
				}

				int close = findCloseBracketFor('(', tmpLine);

				tmpLine = tmpLine.left( close ).trimmed();
				mKeyword += QString("(%1)").arg(tmpLine);

				mLine = mLine.mid( close+1 ).trimmed();
				if (mLine.startsWith(":"))
				{
					item = new ProjectItem( ProjectItem::Variable );
					item ->setData( ":", ProjectItem::OperatorRole );

					parseLine(mLine.mid(1, i), item);
				}
				else
				{
					item = new ProjectItem( ProjectItem::Function );
					item->setData( mComment, ProjectItem::CommentRole );
				}
				item->setData( mKeyword, ProjectItem::ValueRole );
				if (!parent || parent->data(ProjectItem::OperatorRole) != ":" )
					item->setData( indent, ProjectItem::IndentRole );
				//
				appendItem(item, parent);
				return;
			}
		}
	}
}
//
void ProjectItemModel::parseBlock(const QString& block, ProjectItem* parent)
{
	if (block.isEmpty())
		return;
	QRegExp rx;
	rx.setPattern("^([^\{]+)");
	int pos = rx.indexIn(block);
	int blockEnd, blockStart;
	QString text;
	//
	if ( pos != -1)
	{
		pos = rx.matchedLength ();
		blockEnd = findCloseBracketFor('{', block.mid(pos+1));
		QString textInScope = block.mid(pos+1, blockEnd);

		//------------------------------------- parse value in bracket : $${subname}
		if (!textInScope.contains("\n")) //{...}
		{
			if (!block.trimmed().contains("\n"))
			{
				parseLine(block, parent);
				return;
			}
			else //{ ... } ... {
			{
				blockEnd = block.lastIndexOf("}");
				if (blockEnd != -1)
				{
					blockStart = findOpenBracketFor('}', block.left(blockEnd));
					text = block.left(blockStart).trimmed();
					textInScope = block.mid(blockStart+1, blockEnd-blockStart-1).trimmed();
				}
			}
		}
		//------------------------------------- end

		ProjectItem* item = new ProjectItem(ProjectItem::Scope);
		if (text.isEmpty())
			text = rx.cap(1).trimmed();
		text = checkForComment(text, parent);
		item->setData(text, ProjectItem::ValueRole );
		item->setData(indent, ProjectItem::IndentRole );

		appendItem(item, parent);

		QTextStream ts(textInScope.trimmed().toLocal8Bit());

		++indent;
		parseText(ts, item);
		--indent;
		ProjectItem* iEnd = new ProjectItem(ProjectItem::ScopeEnd);
		iEnd->setData( indent, ProjectItem::IndentRole );
		appendItem(iEnd, item);
	}
	else return;

	QTextStream ts(block.mid(pos+blockEnd+2).trimmed().toLocal8Bit());
	parseText(ts, parent);
}
//
void ProjectItemModel::writeToBuffer(QList<ProjectItem*> childList)
{
	QList<ProjectItem*> list;
	if (!childList.isEmpty())
		list = childList;
	else
		list = topLevelItems();
	ProjectItem* first = list.first();
	ProjectItem* last = list.last();
	QList<ProjectItem*>::iterator it = list.begin();
	while (it != list.end())
	{
		bool hasChild = (*it)->hasChildren();
		QString line;
		QString tab = QString().fill('\t',(*it)->data(ProjectItem::IndentRole).toInt());
		switch ((*it)->type())
		{
			case ProjectItem::Line:
			case ProjectItem::Function:
				line = tab+(*it)->data(ProjectItem::ValueRole).toString()+"\n";
				break;
			case ProjectItem::Variable:
				line = tab+(*it)->data(ProjectItem::ValueRole).toString()+
					   QString(" %1 ").arg((*it)->data(ProjectItem::OperatorRole).toString());
				break;
			case ProjectItem::Value:
				{
					if ((*it) == first)
						tab.clear();
					line = tab+(*it)->data(ProjectItem::ValueRole).toString();
					if ((*it) != list.last())
						line += " \\";
					QString comment = (*it)->data(ProjectItem::CommentRole).toString();
					if (!comment.isEmpty())
						line += QString("\t%1").arg(comment);
					line += '\n';
					break;
				}
			case ProjectItem::File:
				if ((*it) == first)
					tab.clear();
				line = tab+(*it)->data(ProjectItem::ValueRole).toString();
				if ((*it) != last)
					line += " \\";
				line += '\n';
				break;
			case ProjectItem::Folder:
				childList.clear();
				while (it != list.end() && (*it)->type() == ProjectItem::Folder)
				{
					childList += (*it)->children();
					++it;
				}
				writeToBuffer(childList);
				return;
			case ProjectItem::Scope:
				line = tab+(*it)->data(ProjectItem::ValueRole).toString()+" {\n";
				break;
			case ProjectItem::ScopeEnd:
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
			writeToBuffer((*it)->children());
		}
		++it;
	}
}
//
void ProjectItemModel::writeToFile()
{
	writeToBuffer();
	//cout <<mFileText.toStdString()<<endl;
	emit sig_writeText(mFileText);
	mFileText.clear();
	indent = 0;
}
//
void ProjectItemModel::saveProject()
{
	writeToFile();
}
//
ProjectItem* ProjectItemModel::getFolder( const QString& filePath, ProjectItem* parent )
{
	if ( filePath.isEmpty() || filePath == "." || filePath == "./" )
		return parent;
	//
	ProjectItem* iFolder = 0;
	iFolder = 0;

	foreach (ProjectItem* item, parent->children())
	{
		// check if itel is the one require
		if ( item->type() == ProjectItem::Folder &&
				item->data( ProjectItem::ValueRole ).toString() == filePath )
			return item;
	}
	// create item if it not exists
	iFolder = new ProjectItem( ProjectItem::Folder );
	iFolder->setData( filePath, ProjectItem::ValueRole);
	appendItem(iFolder, parent);
	//
	return iFolder;
}
//
QList<ProjectItem*> ProjectItemModel::topLevelItems()
{
	ProjectItem* project = currentProject();
	if (project == horizontalHeaderItem(0))
		project = 0;
	//
	QList <ProjectItem*> list;
	int i = 0;
	ProjectItem* item;
	if (!project)
		item = (ProjectItem*)this->item(i);
	else
		item = (ProjectItem*)project->child(i);
	while (item != 0)
	{
		if (item->type() != ProjectItem::Project)
			list<<item;
		if (!project)
			item = (ProjectItem*)this->item(++i);
		else
			item = (ProjectItem*)project->child(++i);
	}
	return list;
}
//
QString ProjectItemModel::checkForComment(QString text, ProjectItem* parent)
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
void ProjectItemModel::appendItem(ProjectItem* item, ProjectItem* parent)
{
	if (parent)
		parent->appendRow(item);
	else
		appendRow(item);
	setItemData( item );
}
//
void ProjectItemModel::setItemData(ProjectItem* item)
{
	item->setData(item->data(ProjectItem::ValueRole).toString(), Qt::DisplayRole);
	// pasnox set correct project
	if ( item->index().parent().isValid() )
		item->setData( item->index().parent().data( ProjectItem::ProjectRole ).toInt(), ProjectItem::ProjectRole );
	else
		item->setData( (uint)mProject, ProjectItem::ProjectRole );
	item->setData(item->row(), ProjectItem::SortRole2);
	switch (item->type())
	{
		case ProjectItem::Line:
			item->setData(QIcon(":/Images/images/line.png"), Qt::DecorationRole);
			item->setData(item->data(ProjectItem::ValueRole).toString(), Qt::ToolTipRole);
			item->setData(1, ProjectItem::SortRole);
			item->setData(false, ProjectItem::FilterRole);
			break;
		case ProjectItem::Variable:
			{
				item->setData(QIcon(":/Images/images/variable.png"), Qt::DecorationRole);
				item->setData(item->data(ProjectItem::ValueRole).toString()+
							  item->data(ProjectItem::OperatorRole).toString(), Qt::ToolTipRole);
				QString keyword = item->data(ProjectItem::ValueRole).toString();
				if (keyword == "HEADERS")
				{
					item->setData(2, ProjectItem::SortRole);
					item->setData(true, ProjectItem::FilterRole);
				}
				else if (keyword == "SOURCES")
				{
					item->setData(3, ProjectItem::SortRole);
					item->setData(true, ProjectItem::FilterRole);
				}
				else if (keyword == "FORMS")
				{
					item->setData(4, ProjectItem::SortRole);
					item->setData(true, ProjectItem::FilterRole);
				}
				else if (keyword == "FORMS3")
				{
					item->setData(5, ProjectItem::SortRole);
					item->setData(true, ProjectItem::FilterRole);
				}
				else if (keyword == "TRANSLATIONS")
				{
					item->setData(6, ProjectItem::SortRole);
					item->setData(true, ProjectItem::FilterRole);
				}
				else
				{
					item->setData(1, ProjectItem::SortRole);
					item->setData(false, ProjectItem::FilterRole);
				}
				break;
			}
		case ProjectItem::Value:
			{
				item->setData(QIcon(":/Images/images/value.png"), Qt::DecorationRole);
				QString line = item->data(ProjectItem::ValueRole).toString();
				QString comment = item->data(ProjectItem::CommentRole).toString();
				if (!comment.isEmpty())
					line += QString("\t%1").arg(comment);
				item->setData(line, Qt::ToolTipRole);
				item->setData(1, ProjectItem::SortRole);
				item->setData(false, ProjectItem::FilterRole);
				break;
			}
		case ProjectItem::File:
			{
				item->setData(QIcon(":/Images/images/value.png"), Qt::DecorationRole);
				QString line = item->data(ProjectItem::ValueRole).toString();
				item->setData(QFileInfo(line).fileName(), Qt::DisplayRole);
				/*QString comment = item->data(ProjectItem::CommentRole).toString();
				if (!comment.isEmpty())
					line += QString("\t%1").arg(comment);*/
				item->setData(item->data(ProjectItem::AbsoluteFilePathRole).toString(), Qt::ToolTipRole);
				item->setData(1, ProjectItem::SortRole);
				item->setData(true, ProjectItem::FilterRole);
				break;
			}
		case ProjectItem::Function:
			item->setData(QIcon(":/Images/images/function.png"), Qt::DecorationRole);
			item->setData(item->data(ProjectItem::ValueRole).toString(), Qt::ToolTipRole);
			item->setData(1, ProjectItem::SortRole);
			item->setData(false, ProjectItem::FilterRole);
			break;
		case ProjectItem::Folder:
			item->setData(QIcon(":/Images/images/folder.png"), Qt::DecorationRole);
			item->setData(item->data(ProjectItem::ValueRole).toString(), Qt::ToolTipRole);
			item->setData(1, ProjectItem::SortRole);
			item->setData(item->row(), ProjectItem::SortRole2);
			item->setData(true, ProjectItem::FilterRole);
			break;
		case ProjectItem::Scope:
			item->setData(QIcon(":/Images/images/scope.png"), Qt::DecorationRole);
			item->setData(item->data(ProjectItem::ValueRole).toString(), Qt::ToolTipRole);
			item->setData(1, ProjectItem::SortRole);
			item->setData(false, ProjectItem::FilterRole);
			break;
		case ProjectItem::ScopeEnd:
			item->setData(QIcon(":/Images/images/scope_end.png"), Qt::DecorationRole);
			item->setData(1, ProjectItem::SortRole);
			item->setData(false, ProjectItem::FilterRole);
			break;
		case ProjectItem::Project:
			item->setData(QFileInfo(item->data(ProjectItem::AbsoluteFilePathRole).toString()).baseName(), Qt::DisplayRole);
			item->setData(QIcon(":/Images/images/project.png"), Qt::DecorationRole);
			item->setData(item->data(ProjectItem::AbsoluteFilePathRole).toString(), Qt::ToolTipRole);
			item->setData(0, ProjectItem::SortRole);
			item->setData(true, ProjectItem::FilterRole);
			break;
	}
}
//-----------------------------------GET----------------------------------------
/*QString ProjectItemModel::getValue( const QString& var, const QStringList& scope)
{
	return getValuesList( var, scope ).value( 0 );
}*/
//
QStringList ProjectItemModel::getValuesList( const QString& var, const QStringList& scope)
{
	QStringList list;
	QList<ProjectItem*> itemList = findItem(var, scope);
	QList<ProjectItem*>::iterator it = itemList.begin();
	while (it != itemList.end())
	{
		if ((*it)->data(ProjectItem::OperatorRole).toString() == "=")
			list.clear();
		foreach(ProjectItem* child, (*it)->children())
		{
			if (child->type() == ProjectItem::Folder)
			{
				foreach(ProjectItem* folderChild, child->children())
				{
					list<<folderChild->data(ProjectItem::ValueRole).toString();
				}
			}
			else
				list<<child->data(ProjectItem::ValueRole).toString();
		}
		++it;
	}
	return list;
}
//
/*QString ProjectItemModel::getValues( const QString& var, const QStringList& scope)
{
	// get list as string
	return getValuesList( var, scope).join( " " );
}*/
//
QStringList ProjectItemModel::getSettingsList(ProjectItem* parent)
{
	QStringList list;
	if (!parent)
		return list;
	QList<ProjectItem*> itemList = parent->children();
	QList<ProjectItem*>::iterator it = itemList.begin();
	while (it != itemList.end())
	{
		list+=getStringList((*it)->data(ProjectItem::ValueRole).toString());
		++it;
	}
	return list;
}
//-----------------------------------SET----------------------------------------
/*
void ProjectItemModel::setValue(  const QString& val, const QString& var, const QStringList& scope)
{
	setValuesList( QStringList()<<val, var, scope);
}
//
void ProjectItemModel::setValuesList( const QStringList& valList, const QString& var, const QStringList& scope)
{
	QStringList list;
	ProjectItem* item = findItem(var, scope);
}
//
void ProjectItemModel::setValues( const QString& val, const QString& var, const QStringList& scope)
{
	// set string as list
	return setValuesList( getStringList(val), var, scope );
}*/
//-------------------------------------------------------------------------------
//
QStringList ProjectItemModel::getStringList(const QString& str)
{
	return str.split(QRegExp("(\\s+|(\\s+\\\\(\\s+)?))"));
}
//
void ProjectItemModel::setSettingsList(ProjectItem* item, const QStringList& list)
{
	if (list.isEmpty())
	{
		clearHash(item);
		QModelIndex i = item->index();
		if ( i.parent().isValid() )
			removeRow( i.row(), i.parent() );
		else
			removeRow( i.row() );
		return;
	}
	removeChilds(item);
	ProjectItem* iValue = 0;
	foreach ( QString s, list )
	{
		iValue = new ProjectItem( ProjectItem::Value );
		iValue->setData( s, ProjectItem::ValueRole );
		iValue->setData(item->data(ProjectItem::IndentRole).toInt()+1, ProjectItem::IndentRole);
		appendItem(iValue, item);
	}
}
//
void ProjectItemModel::clearHash(ProjectItem* item)
{
	ProjectItem* parent = currentProject();
	HashSimpleSettings hash = mProjectList[(uint)parent];
	QString variable = item->text();
	QString op = item->data(ProjectItem::OperatorRole).toString();
	HashSimpleSettings::iterator it = hash.find(variable);
	if  (it != hash.end())
	{
		QHash<QString, uint> hash_ = it.value();
		QHash<QString, uint>::iterator it_ = hash_.find(op);
		while (it_ != hash_.end() && it_.key() == op)
		{
			if (it_.value() == (uint)item)
			{
				//	cout <<qPrintable("clearHash item = "+QString::number((uint)item))<<endl;
				hash_.erase(it_);
				it.value() = hash_;
				mProjectList[(uint)parent] = hash;
				return;
			}
			++it_;
		}
	}
}
//
void ProjectItemModel::removeChilds(ProjectItem* parent)
{
	int rowCount = parent->rowCount();
	QModelIndex index = parent->index();
	for (int i = 0; i<rowCount; ++i)
	{
		removeRow(0, index);
	}
}
//
ProjectItem* ProjectItemModel::addNewItem(const QString& variable, const QString& op)
{
	ProjectItem* item = new ProjectItem(ProjectItem::Variable);
	item->setData(variable, ProjectItem::ValueRole);
	item->setData(op, ProjectItem::OperatorRole);
	item->setData(0, ProjectItem::IndentRole);
	ProjectItem* parent = currentProject();
	HashSimpleSettings::iterator it = mProjectList[(uint)parent].find(variable);
	if( it!= mProjectList[(uint)parent].end())
	{
		it.value().insertMulti(op, (uint)item);
	}
	if (parent == horizontalHeaderItem(0))
		parent = 0;
	appendItem(item, parent);
	return item;
}
//
ProjectItem* ProjectItemModel::currentProject( const QModelIndex& index )
{
	if (index.isValid())
	{
		ProjectItem* item = static_cast<ProjectItem*>( itemFromIndex( index ) );
		return (ProjectItem*)item->data(ProjectItem::ProjectRole).toUInt();
	}
	return mProject;
}
//
QString ProjectItemModel::currentProjectName( const QModelIndex& index )
{
	ProjectItem* project;
	if (!index.isValid())
		project = mProject; //(ProjectItem*)horizontalHeaderItem(0);
	else
	{
		ProjectItem* item = static_cast<ProjectItem*>( itemFromIndex( index ) );
		project = (ProjectItem*)item->data(ProjectItem::ProjectRole).toUInt();
		mProject = project;
	}

	return project->data(Qt::DisplayRole).toString();
}
//
QString ProjectItemModel::currentProjectPath()
{
	if (!mProject)
		return QString();
	return QFileInfo(mProject->data(ProjectItem::AbsoluteFilePathRole).toString()).absolutePath();
}
//
QList<ProjectItem*> ProjectItemModel::findItem(
	const QString& var,
	const QStringList& scope,
	const QString& op)
{
	QList<ProjectItem*> itemsList = topLevelItems();
	QList<ProjectItem*> tmpList;
	if (!scope.isEmpty())
	{
		foreach (QString s, scope)
		{
			foreach(ProjectItem* item, itemsList)
			{
				if (s  == item->data(ProjectItem::ValueRole).toString())
					tmpList += item->children();
			}
			if (tmpList.isEmpty())
				return tmpList;
			itemsList = tmpList;
			tmpList.clear();
		}
	}

	foreach(ProjectItem* item, itemsList)
	{
		if (var  == item->data(ProjectItem::ValueRole).toString())
		{
			tmpList +=item;
		}
	}

	return tmpList;
}
//
int ProjectItemModel::findCloseBracketFor(const QChar& bracket, const QString& text)
{
	//Do not start text with bracket
	// if u have "aaa(bbb())"
	// need call -- findCloseBracketFor('(', "bbb())")
	//--------------------------------------------
	QChar closeBracket;
	switch (bracket.toAscii())
	{
		case '{':
			closeBracket = '}';
			break;
		case '(':
			closeBracket = ')';
			break;
	}
	int counter = 0;
	for (int i = 0; i < text.length(); ++i)
	{
		if (text.at(i) == bracket)
			++counter;
		else if (text.at(i) == closeBracket)
		{
			if (!counter)
				return i;
			else
				--counter;
		}
	}
	return -1;
}
//
int ProjectItemModel::findOpenBracketFor(const QChar& bracket, const QString& text)
{
	//Do not start text with bracket
	// if u have "aaa( bbb() )"
	// need call -- findOpenBracketFor(')', "aaa( bbb() ")
	//--------------------------------------------
	QChar openBracket;
	switch (bracket.toAscii())
	{
		case '}':
			openBracket = '{';
			break;
		case ')':
			openBracket = '(';
			break;
	}
	int counter = 0;
	for (int i = text.length(); i > 0; --i)
	{
		if (text.at(i) == bracket)
			++counter;
		else if (text.at(i) == openBracket)
		{
			if (!counter)
				return i;
			else
				--counter;
		}
	}
	return -1;
}
//
//----------------------------------------------------------------------
//																		OLD
//----------------------------------------------------------------------
/*void ProjectItemModel::parseBlock(const QString& block, ProjectItem* parent)
{
	if (block.isEmpty())
		return;
	int opened1 = block.indexOf("{");
	int opened2 = block.indexOf("{", opened1+1);
	int closed = block.indexOf("}", opened1+1);
	
	if (closed != -1 && (closed<opened2 || (opened2 == -1 && opened1 != -1)))  //{ ...\n ..}
	{
		parseSimpleBlock(block.left(closed+1), parent);
		parseBlock(block.mid(closed+1).trimmed(), parent);
	}
	else if (opened2 != -1 && closed>opened2 )
	{
		ProjectItem* item = new ProjectItem(ProjectItem::Scope);
		QString text = checkForComment(block.left(opened1).trimmed(), parent);
		item->setData(text, ProjectItem::ValueRole);
		item->setData(indent, ProjectItem::IndentRole );
		complexScopeList<<item;

		appendItem(item, parent);
		++indent;
		parseBlock(block.mid(opened1+1).trimmed(), item);
	}
	else if (!closed && opened1 == -1 && opened2 == -1)
	{
		complexScopeList.removeLast();
		ProjectItem* iEnd = new ProjectItem(ProjectItem::ScopeEnd);
		--indent;
		iEnd->setData( indent, ProjectItem::IndentRole );

		if (!complexScopeList.isEmpty())
			parent = complexScopeList.last();
		else
			parent = 0;
		appendItem(iEnd, parent);
		parseBlock(block.mid(closed+1).trimmed(), parent);
	}
}
//
void ProjectItemModel::parseSimpleBlock(const QString& block, ProjectItem* parent)
{
	QRegExp rx;
	rx.setPattern("^([^\{]+)\\{([^\}]+)");
	int pos = rx.indexIn(block);
	if ( pos != -1)
	{
		ProjectItem* item = new ProjectItem(ProjectItem::Scope);
		QString text = checkForComment(rx.cap(1).trimmed(), parent);
		item->setData(text, ProjectItem::ValueRole );
		item->setData(indent, ProjectItem::IndentRole );

		appendItem(item, parent);
		QTextStream ts(rx.cap(2).trimmed().toLocal8Bit());
		++indent;
		parseText(ts, item);
		--indent;
		ProjectItem* iEnd = new ProjectItem(ProjectItem::ScopeEnd);
		iEnd->setData( indent, ProjectItem::IndentRole );
		appendItem(iEnd, item);
	}
}*/
//----------------------------------------------------------------------
//																END 	OLD
//----------------------------------------------------------------------