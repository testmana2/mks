#ifndef QCTAGSSENSE_H
#define QCTAGSSENSE_H

#include <QObject>
#include <QMap>
#include <QPair>

class qCtagsSenseSQL;
class qCtagsSenseIndexer;

// Fileds
/*
	flags: "afmikKlnsStz") [fks]. 
*/

// Kinds
/*
Asm                                        
	d  defines                             
	l  labels                              
	m  macros                              
	t  types (structs and records)         
Asp                                        
	c  constants                           
	f  functions                           
	s  subroutines                         
	v  variables                           
Awk                                        
	f  functions                           
Basic                                      
	c  constants                           
	f  functions                           
	l  labels                              
	t  types                               
	v  variables                           
	g  enumerations                        
BETA                                       
	f  fragment definitions                
	p  all patterns [off]                  
	s  slots (fragment uses)               
	v  patterns (virtual or rebound)       
C / C++                                        
	c  classes                             
	d  macro definitions                   
	e  enumerators (values inside an enumeration)
	f  function definitions                      
	g  enumeration names                         
	l  local variables [off]                     
	m  class, struct, and union members          
	n  namespaces                                
	p  function prototypes [off]                 
	s  structure names                           
	t  typedefs                                  
	u  union names                               
	v  variable definitions                      
	x  external and forward variable declarations [off]
C#                                                     
	c  classes                                         
	d  macro definitions                               
	e  enumerators (values inside an enumeration)      
	E  events                                          
	f  fields                                          
	g  enumeration names                               
	i  interfaces                                      
	l  local variables [off]                           
	m  methods                                         
	n  namespaces                                      
	p  properties                                      
	s  structure names                                 
	t  typedefs                                        
Cobol                                                  
	d  data items                                      
	f  file descriptions (FD, SD, RD)                  
	g  group items                                     
	p  paragraphs                                      
	P  program ids                                     
	s  sections                                        
Eiffel                                                 
	c  classes                                         
	f  features                                        
	l  local entities [off]                            
Erlang                                                 
	d  macro definitions                               
	f  functions                                       
	m  modules                                         
	r  record definitions                              
Fortran                                                
	b  block data                                      
	c  common blocks                                   
	e  entry points                                    
	f  functions                                       
	i  interface contents, generic names, and operators [off]
	k  type and structure components                         
	l  labels                                                
	L  local, common block, and namelist variables [off]     
	m  modules                                               
	n  namelists                                             
	p  programs                                              
	s  subroutines                                           
	t  derived types and structures                          
	v  program (global) and module variables                 
HTML                                                         
	a  named anchors                                         
	f  JavaScript functions                                  
Java                                                         
	c  classes                                               
	e  enum constants                                        
	f  fields                                                
	g  enum types                                            
	i  interfaces                                            
	l  local variables [off]                                 
	m  methods                                               
	p  packages                                              
JavaScript                                                   
	f  functions                                             
	c  classes                                               
	m  methods                                               
	v  global variables                                      
Lisp                                                         
	f  functions                                             
Lua                                                          
	f  functions                                             
Make                                                         
	m  macros                                                
Pascal                                                       
	f  functions                                             
	p  procedures                                            
Perl                                                         
	c  constants                                             
	f  formats                                               
	l  labels                                                
	p  packages                                              
	s  subroutines                                           
	d  subroutine declarations [off]                         
PHP                                                          
	c  classes                                               
	i  interfaces                                            
	d  constant definitions                                  
	f  functions                                             
	v  variables                                             
	j  javascript functions                                  
	j  javascript functions                                  
	j  javascript functions                                  
Python                                                       
	c  classes                                               
	f  functions                                             
	m  class members                                         
REXX                                                         
	s  subroutines                                           
Ruby                                                         
	c  classes                                               
	f  methods                                               
	m  modules                                               
	F  singleton methods                                     
Scheme                                                       
	f  functions                                             
	s  sets                                                  
Sh                                                           
	f  functions                                             
SLang                                                        
	f  functions                                             
	n  namespaces                                            
SML                                                          
	e  exception declarations                                
	f  function definitions                                  
	c  functor definitions                                   
	s  signature declarations                                
	r  structure declarations                                
	t  type definitions                                      
	v  value bindings                                        
SQL                                                          
	c  cursors                                               
	d  prototypes [off]                                      
	f  functions                                             
	F  record fields                                         
	l  local variables [off]                                 
	L  block label                                           
	P  packages                                              
	p  procedures                                            
	r  records [off]                                         
	s  subtypes                                              
	t  tables                                                
	T  triggers                                              
	v  variables                                             
	i  indexes                                               
	e  events                                                
	U  publications                                          
	R  services                                              
	D  domains                                               
	V  views                                                 
	n  synonyms                                              
	x  MobiLink Table Scripts                                
	y  MobiLink Conn Scripts                                 
Tcl                                                          
	c  classes                                               
	m  methods
	p  procedures
Vera
	c  classes
	d  macro definitions
	e  enumerators (values inside an enumeration)
	f  function definitions
	g  enumeration names
	l  local variables [off]
	m  class, struct, and union members
	p  programs
	P  function prototypes [off]
	t  tasks
	T  typedefs
	v  variable definitions
	x  external variable declarations [off]
Verilog
	c  constants (define, parameter, specparam)
	e  events
	f  functions
	m  modules
	n  net data types
	p  ports
	r  register data types
	t  tasks
Vim
	a  autocommand groups
	c  user-defined commands
	f  function definitions
	m  maps
	v  variable definitions
YACC
	l  labels
*/

class qCtagsSense : public QObject
{
	Q_OBJECT
	
public:
	enum Kind
	{
		Unknow = -1,
		Class, // c
		Macro, // d
		Enumerator, // e
		Function, // f
		Enum, // g
		LocalVariable, // l
		Member, // m
		Namespace, // n
		Prototype, // p
		Structure, // s
		Typedef, // t
		Union, // u
		Variable, // v
		ExternVariable // x
	};
	
	qCtagsSense( QObject* parent = 0 );
	virtual ~qCtagsSense();
	
	bool isValid() const;
	qCtagsSenseSQL* sql() const;
	qCtagsSenseIndexer* indexer() const;
	
	static qCtagsSense::Kind kindType( const QChar& c );
	static QChar kindChar( qCtagsSense::Kind kind );
	static QString kindText( qCtagsSense::Kind kind );
	static QString kindText( const QChar& c );

public slots:
	void tagEntry( const QString& fileName );
	void tagEntries( const QMap<QString, QString>& entries );

protected:
	static QMap<qCtagsSense::Kind, QChar> mKindChar;
	static QMap<QChar, qCtagsSense::Kind> mCharKind;
	static QMap<qCtagsSense::Kind, QString> mKindText;
	bool mInitialized;
	qCtagsSenseSQL* mSQL;
	qCtagsSenseIndexer* mIndexer;

signals:
	void indexChanged();
};

struct qCtagsSenseEntry;
typedef QList<qCtagsSenseEntry*> qCtagsSenseEntryList;

struct qCtagsSenseEntry
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

struct qCtagsSenseFile
{
	~qCtagsSenseFile()
	{
		qDeleteAll( entries );
	}
	
	QString fileName;
	QString language;
	qCtagsSenseEntryList entries;
};

#endif // QCTAGSSENSE_H
