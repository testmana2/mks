/****************************************************************************
	Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef QCTAGSSENSE_H
#define QCTAGSSENSE_H

#include "qCtagsSenseExport.h"

#include <QObject>
#include <QMap>
#include <QPair>
#include <QStringList>
#include <QMetaType>

class qCtagsSenseSQL;
class qCtagsSenseIndexer;

struct QCTAGSSENSE_EXPORT qCtagsSenseProperties
{
	qCtagsSenseProperties( const QStringList& systemPaths = QStringList(), const QStringList filteredSuffixes = QStringList(),
		bool usePhysicalDatabase = false, const QString& databaseFileName = QString::null )
	{
		SystemPaths = systemPaths;
		FilteredSuffixes = filteredSuffixes;
		UsePhysicalDatabase = usePhysicalDatabase;
		DatabaseFileName = databaseFileName;
	}
	
	bool operator==( const qCtagsSenseProperties& other ) const
	{
		return SystemPaths == other.SystemPaths && FilteredSuffixes == other.FilteredSuffixes &&
			UsePhysicalDatabase == other.UsePhysicalDatabase && DatabaseFileName == other.DatabaseFileName;
	}
	
	bool operator!=( const qCtagsSenseProperties& other ) const
	{
		return !operator==( other );
	}
	
	QStringList SystemPaths;
	QStringList FilteredSuffixes;
	bool UsePhysicalDatabase;
	QString DatabaseFileName;
};

class QCTAGSSENSE_EXPORT qCtagsSense : public QObject
{
	Q_OBJECT
	
public:
	enum Kind
	{
		Unknow = -1,
		// A
		AllPatterns, // BETA 0
		Anchor, // HTML 1
		AutoCommandGroup, // Vim 2
		// B
		BlockData, // Fortran 3
		BlockCommon, // Fortran 4
		// C
		Class, // Vera, Tcl, Ruby, Python, PHP, JavaScript, Java, Eiffel, C#, C, C++ 5
		Constant, // Verilog, PHP, Perl, Basic, Asp 6
		Cursor, // SQL 7
		// D
		Define, // Asm 8
		DerivatedType, // Fortran 9
		Domain, // SQL 10
		// E
		EntryPoint, // Fortran 11
		Enumeration, // Vera, Java, C#, C, C++, Basic 12
		Enumerator, // Vera, C#, C, C++ 13
		EnumType, // Java 14
		Event, // Verilog, SQL, C# 15
		Exception, // SML 16
		// F
		Feature, // Eiffel 17
		Field, // Java, C#, SQL 18
		FileDescriptor, // Cobol 19
		Format, // Perl 20
		Fragment, // BETA 21
		Function, // Vim, Verilog, Vera, SQL, SML, SLang, Sh, Scheme, Python, PHP, Pascal, Lua, Lisp, JavaScript, Fortran, Erlang, C, C++, Basic, Awk, Asp 22
		Functor, // SML 23
		// G
		// H
		// I
		Index, // SQL 24
		Interface, // PHP, Java, Fortran, C# 25
		ItemData, // Cobol 26
		ItemGroup, // Cobol 27
		// J
		JavaScriptFunction, // PHP, HTML 28
		// K
		// L
		Label, // YACC, SQL, Perl, Fortran, Basic, Asm 29
		// M
		Macro, // Vera, Make, Erlang, C#, C, C++, Asm 30
		Map, // Vim 31
		Member, // Vera, Python, C, C++ 32
		Method, // Tcl, Ruby, JavaScript, Java, C# 33
		MethodSingleton, // Ruby 34
		MobiLinkTableScript, // SQL 35
		MobiLinkConnScript, // SQL 36
		Module, // Verilog, Ruby, Fortran, Erlang 37
		// N
		NameList, // Fortran 38
		Namespace, // SLang, C#, C, C++ 39
		NetDataType, // Verilog 40
		// O
		// P
		Package, // SQL, Perl, Java 41
		Paragraph, // Cobol 42
		Pattern, // BETA 43
		Port, // Verilog 44
		Procedure, // Tcl, SQL, Pascal 45
		Program, // Vera, Fortran 46
		ProgramId, // Cobol 47
		Property, // C# 48
		Prototype, // Vera, SQL, C, C++ 49
		Publication, // SQL 50
		// Q
		// R
		Record, // SQL, Erlang 51
		RegisterDataType, // Verilog 52
		// S
		Section, // Cobol 53
		Service, // SQL 54
		Set, // Scheme 55
		Signature, // SML 56
		Slot, // BETA 57
		SubRoutine, // REXX, Perl, Fortran, Asp 58
		SubRoutineDeclaration, // Perl 59
		SubType, // SQL 60
		Structure, // SML, C#, C, C++ 61
		Synonym, // SQL 62
		// T
		Table, // SQL 63
		Task, // Verilog, Vera 64
		Trigger, // SQL 65
		Type, // SML, Fortran, Basic, Asm 66
		Typedef, // Vera, C#, C, C++ 67
		// U
		Union, // C, C++ 68
		UserDefinedCommand, // Vim 69
		// V
		ValueBinding, // SML 70
		Variable, // Vim, Vera, SQL, PHP, Fortran, C, C++, Basic, Asp 71
		VariableLocal, // Vera, SQL, Java, Fortran, Eiffel, C#, C, C++ 72
		VariableGlobal, // JavaScript, Vera, C, C++ 73
		View, // SQL 74
		// W
		// X
		// Y
		// Z
	};
	
	enum Language {
		UnknowLanguage = -1,
		Asm,
		Asp,
		Awk,
		Basic,
		BETA,
		C,
		Cpp,
		Csharp,
		Cobol,
		Eiffel,
		Erlang,
		Fortran,
		HTML,
		Java,
		JavaScript,
		Lisp,
		Lua,
		Make,
		Pascal,
		Perl,
		PHP,
		Python,
		REXX,
		Ruby,
		Scheme,
		Sh,
		SLang,
		SML,
		SQL,
		Tcl,
		Vera,
		Verilog,
		Vim,
		YACC,
	};
	
	enum AccessFilter {
		All,
		Protected,
		Public,
	};
	
	qCtagsSense( QObject* parent = 0, const QString& dbName = QString("qCtagsSenseSQL") );
	virtual ~qCtagsSense();
	
	void setCtagsLanguageKinds( const char* const language, const char* kinds );
	void setAccessFilter( AccessFilter access );
	bool isValid() const;
	qCtagsSenseProperties properties() const;
	qCtagsSenseSQL* sql() const;
	qCtagsSenseIndexer* indexer() const;

public slots:
	void setProperties( const qCtagsSenseProperties& properties );
	void tagEntry( const QString& fileName );
	void tagEntries( const QStringList& fileNames );
	void tagEntries( const QMap<QString, QString>& entries );

protected:
	bool mInitialized;
	qCtagsSenseProperties mProperties;
	qCtagsSenseSQL* mSQL;
	qCtagsSenseIndexer* mIndexer;

signals:
	void indexingStarted();
	void indexingProgress( int value, int max );
	void indexingFinished();
	void indexingChanged();
};

struct qCtagsSenseEntry;
typedef QList<qCtagsSenseEntry*> qCtagsSenseEntryList;

struct QCTAGSSENSE_EXPORT qCtagsSenseEntry
{
	qCtagsSenseEntry( qCtagsSenseEntry* pItem = 0, bool numberEntry = false, ulong number = -1,  bool scope = false, bool entry = false, bool truncate = false, qCtagsSense::Kind _kind = qCtagsSense::Unknow )
	{
		parent = pItem;
		lineNumberEntry = numberEntry;
		lineNumber = number;
		isFileScope = scope;
		isFileEntry = entry;
		truncateLine = truncate;
		kind = _kind;
	}
	
	~qCtagsSenseEntry()
	{
		qDeleteAll( children );
	}
	
	qCtagsSenseEntry* parent;
	
	QString fileName;
	uint language;
	bool lineNumberEntry;
	ulong lineNumber;
	bool isFileScope; // is header file
	bool isFileEntry;
	bool truncateLine;
	QString name;
	qCtagsSense::Kind kind;
	// extended fields
	QString access;
	QString fileScope;
	QString implementation;
	QString inheritance;
	QPair<QString, QString> scope;
	QString signature;
	QPair<QString, QString> typeRef;
	// children
	qCtagsSenseEntryList children;
};

Q_DECLARE_METATYPE( qCtagsSenseEntry* )
Q_DECLARE_METATYPE( qCtagsSenseEntry )

#endif // QCTAGSSENSE_H
