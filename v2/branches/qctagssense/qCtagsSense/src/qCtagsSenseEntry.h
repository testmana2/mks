#ifndef QCTAGSSENSEENTRY_H
#define QCTAGSSENSEENTRY_H

#include "qCtagsSenseExport.h"

#include <QList>
#include <QPair>
#include <QSqlRecord>
#include <QMetaType>

struct qCtagsSenseEntry;
typedef QList<qCtagsSenseEntry*> qCtagsSenseEntryList;

struct QCTAGSSENSE_EXPORT qCtagsSenseEntry
{
	enum Kind
	{
		Unknow = -1,
		// A
		AllPatterns, // BETA
		Anchor, // HTML
		AutoCommandGroup, // Vim
		// B
		BlockData, // Fortran
		BlockCommon, // Fortran
		// C
		Class, // Vera, Tcl, Ruby, Python, PHP, JavaScript, Java, Eiffel, C#, C, C++
		Constant, // Verilog, PHP, Perl, Basic, Asp
		Cursor, // SQL
		// D
		Define, // Asm
		DerivatedType, // Fortran
		Domain, // SQL
		// E
		EntryPoint, // Fortran
		Enumeration, // Vera, Java, C#, C, C++, Basic
		Enumerator, // Vera, C#, C, C++
		EnumType, // Java
		Event, // Verilog, SQL, C#
		Exception, // SML
		// F
		Feature, // Eiffel
		Field, // Java, C#, SQL
		FileDescriptor, // Cobol
		Format, // Perl
		Fragment, // BETA
		Function, // Vim, Verilog, Vera, SQL, SML, SLang, Sh, Scheme, Python, PHP, Pascal, Lua, Lisp, JavaScript, Fortran, Erlang, C, C++, Basic, Awk, Asp
		Functor, // SML
		// G
		// H
		// I
		Index, // SQL
		Interface, // PHP, Java, Fortran, C#
		ItemData, // Cobol
		ItemGroup, // Cobol
		// J
		JavaScriptFunction, // PHP, HTML
		// K
		// L
		Label, // YACC, SQL, Perl, Fortran, Basic, Asm
		// M
		Macro, // Vera, Make, Erlang, C#, C, C++, Asm
		Map, // Vim
		Member, // Vera, Python, C, C++
		Method, // Tcl, Ruby, JavaScript, Java, C#
		MethodSingleton, // Ruby
		MobiLinkTableScript, // SQL
		MobiLinkConnScript, // SQL
		Module, // Verilog, Ruby, Fortran, Erlang
		// N
		NameList, // Fortran
		Namespace, // SLang, C#, C, C++
		NetDataType, // Verilog
		// O
		// P
		Package, // SQL, Perl, Java
		Paragraph, // Cobol
		Pattern, // BETA
		Port, // Verilog
		Procedure, // Tcl, SQL, Pascal
		Program, // Vera, Fortran
		ProgramId, // Cobol
		Property, // C#
		Prototype, // Vera, SQL, C, C++
		Publication, // SQL
		// Q
		// R
		Record, // SQL, Erlang
		RegisterDataType, // Verilog
		// S
		Section, // Cobol
		Service, // SQL
		Set, // Scheme
		Signature, // SML
		Slot, // BETA
		SubRoutine, // REXX, Perl, Fortran, Asp
		SubRoutineDeclaration, // Perl
		SubType, // SQL
		Structure, // SML, C#, C, C++
		Synonym, // SQL
		// T
		Table, // SQL
		Task, // Verilog, Vera
		Trigger, // SQL
		Type, // SML, Fortran, Basic, Asm
		Typedef, // Vera, C#, C, C++
		// U
		Union, // C, C++
		UserDefinedCommand, // Vim
		// V
		ValueBinding, // SML
		Variable, // Vim, Vera, SQL, PHP, Fortran, C, C++, Basic, Asp
		VariableLocal, // Vera, SQL, Java, Fortran, Eiffel, C#, C, C++
		VariableGlobal, // JavaScript, Vera, C, C++
		View, // SQL
		// W
		// X
		// Y
		// Z
	};
	
	qCtagsSenseEntry( qCtagsSenseEntry* _p = 0, bool _lne = false, ulong _ln = -1,  bool _fs = false, bool _fe = false, bool _tl = false, qCtagsSenseEntry::Kind _k = qCtagsSenseEntry::Unknow );
	qCtagsSenseEntry( const QSqlRecord& record, const QString& fileName = QString::null );
	virtual ~qCtagsSenseEntry();
	
	qCtagsSenseEntry* parent;
	
	QString fileName;
	QString language;
	bool lineNumberEntry;
	ulong lineNumber;
	bool isFileScope; // is header file
	bool isFileEntry;
	bool truncateLine;
	QString name;
	qCtagsSenseEntry::Kind kind;
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

#endif // QCTAGSSENSEENTRY_H
