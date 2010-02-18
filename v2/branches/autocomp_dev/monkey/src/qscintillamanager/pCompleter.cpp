#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>
#include <QSqlQuery>
#include <QStringList>

#include "pCompleter.h"

pCompleter::pCompleter(QString filename, QPoint cursorPos)
{
	this->cursorPos = cursorPos;
	this->filename = filename;
	
	sqlBase = new qCtagsSenseSQL();
}

pCompleter::~pCompleter()
{
}

void pCompleter::setCursorPos(QPoint pos)
{
	this->cursorPos = pos;
}

QString pCompleter::getStrToAnalyze()
{
	return strToAnalyze;
}
bool pCompleter::isPointer(QString lineToAnalyze)
{
	parse(lineToAnalyze);
	analyze();
	
	return bIsPointer;
}

QStringListModel* pCompleter::autoComplete(QString lineToAnalyze)
{
	QStringListModel *model = new QStringListModel();
	QStringList list;

	qCtagsSenseEntry* scope = 0;
	QSqlQuery q = sqlBase->query();
	q.setForwardOnly( true );
	
	parse(lineToAnalyze);
	analyze();
	
	const QString sql = QString(
		"SELECT e.*, f.fileName FROM entries as e, files as f "
		"WHERE f.id = e.file_id "
		"AND e.name = '%1' "
		"AND e.kind = '5'"
	).arg(preciseType);
	
	qDebug() << sql;
	q.exec( sql );
	
	qDebug() << "Current file : " << filename;
	while ( q.next() )
	{
		scope = qCtagsSenseUtils::entryForRecord( q.record(), QString::null );
		
		if(scope)
		{
			const QString sql = QString(
				"SELECT e.*, f.fileName FROM entries as e, files as f "
				"WHERE f.id = e.file_id "
				"AND e.access = 'public' "
				"AND f.fileName = '%1'"
			).arg(scope->fileName);
			
			qDebug() << sql;
			q.exec( sql );
			
			qDebug() << "Current file : " << filename;
			while ( q.next() )
			{
				scope = qCtagsSenseUtils::entryForRecord( q.record(), QString::null );
				
				if(scope)
				{
					if(!list.contains(scope->name))
						list << scope->name;
					qDebug() << "In list for autocomplete : " << scope->name;
				}
			}
		}
	}
	
	model->setStringList(list);
	
	return model;
}

void pCompleter::parse(QString lineToAnalyze)
{	
	qDebug() << "Read line : " << lineToAnalyze;
	
	lineToAnalyze.replace("\t", "    ");
	lineToAnalyze.truncate(cursorPos.x());
	lineToAnalyze.replace("    ", "");
	
	qDebug() << "Replaced line : " << lineToAnalyze;
	qDebug() << "Cursor : " << cursorPos.x();
	
	QRegExp rx(".*\\W*(\\w+)(\\.|->)?$");
	
	if(rx.exactMatch(lineToAnalyze))
	{
		strToAnalyze = rx.cap(1);
		typeToAnalyze = qCtagsSense::VariableLocal;
	}
	else
	{
		rx.setPattern(".*(\\w+)\\((\\w+(->)?\\w+(\\([.^(]*\\))?)*\\)(\\.|->)?$");
		
		if(rx.exactMatch(lineToAnalyze))
		{
			strToAnalyze = rx.cap(1);
			typeToAnalyze = qCtagsSense::Function;
		}
		else
		{
			rx.setPattern(".*\\W*\\((.*)\\)(\\.|->)?$");
			
			if(rx.exactMatch(lineToAnalyze))
			{
				strToAnalyze = rx.cap(1);
				typeToAnalyze = qCtagsSense::Unknow;
			}
		}
	}
	
	qDebug() << "ok";
	qDebug() << "The word is : " << strToAnalyze;
}

void pCompleter::analyze()
{
	qCtagsSenseEntry* scope = 0;
	QSqlQuery q = sqlBase->query();
	q.setForwardOnly( true );
	
	const QString sql = QString(
		"SELECT e.*, f.fileName FROM entries as e, files as f "
		"WHERE f.id = e.file_id "
		"AND e.name = '%1' "
		"AND f.fileName = '%2'"
	).arg(strToAnalyze).arg(filename);
	
	qDebug() << sql;
	q.exec( sql );
	
	qDebug() << "Current file : " << filename;
	while ( q.next() )
	{
		scope = qCtagsSenseUtils::entryForRecord( q.record(), QString::null );
		
		if(scope){
			if(scope->kind == qCtagsSense::VariableLocal)
			{
				if(variableRange(scope->lineNumber, cursorPos.y()))
				{
					qDebug() << "In local range";
					qDebug() << "Match line : " << strToAnalyze;
					
					patternMatching(strToAnalyze, lineAt(scope->lineNumber));
					
					break;
				}
			}
			else if(scope->kind == qCtagsSense::Function)
			{
				QRegExp rx(".*\\.cpp$");
				
				if(rx.exactMatch(filename))
				{
					QString filenameHeader(filename);
					filenameHeader.replace(".cpp", ".h");
					
					const QString sql = QString(
						"SELECT e.*, f.fileName FROM entries as e, files as f "
						"WHERE f.id = e.file_id "
						"AND e.name = '%1' "
						"AND f.fileName = '%2'"
					).arg(strToAnalyze).arg(filenameHeader);
					
					qDebug() << sql;
					q.exec( sql );
					
					qDebug() << "Current file : " << filename;
					while ( q.next() )
					{
						scope = qCtagsSenseUtils::entryForRecord( q.record(), QString::null );
						
						if(scope){
							qDebug() << "In header";
							qDebug() << "Match line : " << strToAnalyze;
					
							patternMatching(strToAnalyze, lineAt(scope->lineNumber));
							
							break;
						}
						
						scope = 0;
					}
				}
			}
		}
		
		scope = 0;
	}
}

bool pCompleter::variableRange(quint32 declVarLine, quint32 variableLine)
{
	quint32 funcLine = 99999;
	qCtagsSenseEntry* scope = 0;
	QSqlQuery q = sqlBase->query();
	q.setForwardOnly( true );
	
	const QString sql = QString(
		"SELECT e.*, f.fileName FROM entries as e, files as f "
		"WHERE f.id = e.file_id "
		"AND e.kind = '22' "
		"AND f.fileName = '%2'"
	).arg(filename);
	
	qDebug() << sql;
	q.exec( sql );
	
	while ( q.next() )
	{
		scope = qCtagsSenseUtils::entryForRecord( q.record(), QString::null );
		
		if(scope){
			qDebug() << scope->name;
			
			if(scope->lineNumber < variableLine)
				funcLine = scope->lineNumber;
			else
			{
				if(funcLine < declVarLine && declVarLine <= variableLine)
					return true;
				else
					return false;
			}
		}
		
		scope = 0;
	}
	
	return false;
}

void pCompleter::patternMatching(QString strToAnalyze, QString lineToMatch)
{
	QRegExp rx(QString(".*\\s(\\w+)\\s*\\*\\s*%1.*").arg(strToAnalyze));
	
	if(rx.exactMatch(lineToMatch))
	{
		bIsPointer = true;
		preciseType = rx.cap(1);
		qDebug() << "Pointer of type : " << preciseType;
	}
	else
	{
		rx.setPattern(QString(".*\\s*(\\w+)\\s*%1.*").arg(strToAnalyze));
		
		if(rx.exactMatch(lineToMatch))
			preciseType = rx.cap(1);
		else
			preciseType = "";
		
		bIsPointer = false;
		qDebug() << "Not a pointer of type : " << preciseType;
	}
}

QString pCompleter::lineAt(int index, QString filename)
{
	if(filename.isEmpty())
		filename = this->filename;
	
	QString retVal;
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	QTextStream data(&file);
	
	int counter = 0;
	
	while(counter != index)
	{
		data.readLine();
		counter++;
	}
	
	retVal = data.readLine();
	
	file.close();
	
	return retVal;
}
