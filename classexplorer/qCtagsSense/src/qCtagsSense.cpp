#include "qCtagsSense.h"
#include "qCtagsSenseSQL.h"
#include "qCtagsSenseIndexer.h"

#include <ctags.h>

#include <QDebug>

QMap<qCtagsSense::Kind, QChar> qCtagsSense::mKindChar;
QMap<QChar, qCtagsSense::Kind> qCtagsSense::mCharKind;
QMap<qCtagsSense::Kind, QString> qCtagsSense::mKindText;

qCtagsSense::qCtagsSense( QObject* parent )
	: QObject( parent )
{
	if ( mKindChar.isEmpty() )
	{
		mKindChar[ qCtagsSense::Class ] = 'c';
		mKindChar[ qCtagsSense::Macro ] = 'd';
		mKindChar[ qCtagsSense::Enumerator ] = 'e';
		mKindChar[ qCtagsSense::Function ] = 'f';
		mKindChar[ qCtagsSense::Enum ] = 'g';
		mKindChar[ qCtagsSense::LocalVariable ] = 'l';
		mKindChar[ qCtagsSense::Member ] = 'm';
		mKindChar[ qCtagsSense::Namespace ] = 'n';
		mKindChar[ qCtagsSense::Prototype ] = 'p';
		mKindChar[ qCtagsSense::Structure ] = 's';
		mKindChar[ qCtagsSense::Typedef ] = 't';
		mKindChar[ qCtagsSense::Union ] = 'u';
		mKindChar[ qCtagsSense::Variable ] = 'v';
		mKindChar[ qCtagsSense::ExternVariable ] = 'x';
		
		mCharKind[ 'c' ] = qCtagsSense::Class;
		mCharKind[ 'd' ] = qCtagsSense::Macro;
		mCharKind[ 'e' ] = qCtagsSense::Enumerator;
		mCharKind[ 'f' ] = qCtagsSense::Function;
		mCharKind[ 'g' ] = qCtagsSense::Enum;
		mCharKind[ 'l' ] = qCtagsSense::LocalVariable;
		mCharKind[ 'm' ] = qCtagsSense::Member;
		mCharKind[ 'n' ] = qCtagsSense::Namespace;
		mCharKind[ 'p' ] = qCtagsSense::Prototype;
		mCharKind[ 's' ] = qCtagsSense::Structure;
		mCharKind[ 't' ] = qCtagsSense::Typedef;
		mCharKind[ 'u' ] = qCtagsSense::Union;
		mCharKind[ 'v' ] = qCtagsSense::Variable;
		mCharKind[ 'x' ] = qCtagsSense::ExternVariable;
		
		mKindText[ qCtagsSense::Class ] = tr( "Class" );
		mKindText[ qCtagsSense::Macro ] = tr( "Macro" );
		mKindText[ qCtagsSense::Enumerator ] = tr( "Enumerator" );
		mKindText[ qCtagsSense::Function ] = tr( "Function" );
		mKindText[ qCtagsSense::Enum ] = tr( "Enum" );
		mKindText[ qCtagsSense::LocalVariable ] = tr( "Local Variable" );
		mKindText[ qCtagsSense::Member ] = tr( "Member" );
		mKindText[ qCtagsSense::Namespace ] = tr( "Namespace" );
		mKindText[ qCtagsSense::Prototype ] = tr( "Prototype" );
		mKindText[ qCtagsSense::Structure ] = tr( "Structure" );
		mKindText[ qCtagsSense::Typedef ] = tr( "Typedef" );
		mKindText[ qCtagsSense::Union ] = tr( "Union" );
		mKindText[ qCtagsSense::Variable ] = tr( "Variable" );
		mKindText[ qCtagsSense::ExternVariable ] = tr( "Extern Variable" );
		mKindText[ qCtagsSense::Unknow ] = tr( "Unknow" );
	}
	
	initCtags();
	
	Option.include.fileNames = 1;
	Option.include.qualifiedTags = 0;
	Option.include.fileScope = 1;
	
	Option.extensionFields.access = 1;
	Option.extensionFields.fileScope = 1;
	Option.extensionFields.implementation = 1;
	Option.extensionFields.inheritance = 1;
	Option.extensionFields.kind = 1;
	Option.extensionFields.kindKey = 1;
	Option.extensionFields.kindLong = 1;
	Option.extensionFields.language = 1;
	Option.extensionFields.lineNumber = 1;
	Option.extensionFields.scope = 1;
	Option.extensionFields.signature = 1;
	Option.extensionFields.typeRef = 1;
	
	Option.if0 = 1;
	Option.kindLong = 1;
	
	//setLanguageKinds( "C++", "cdefglmnpstuvx" );
	
	mSQL = new qCtagsSenseSQL( this );
	mIndexer = new qCtagsSenseIndexer( mSQL );
	
	mInitialized = mSQL->initializeDatabase();
	
	connect( mIndexer, SIGNAL( indexChanged() ), this, SIGNAL( indexChanged() ) );
}

qCtagsSense::~qCtagsSense()
{
	deInitCtags();
}

bool qCtagsSense::isValid() const
{
	return mInitialized;
}

qCtagsSenseSQL* qCtagsSense::sql() const
{
	return mSQL;
}

qCtagsSenseIndexer* qCtagsSense::indexer() const
{
	return mIndexer;
}

qCtagsSense::Kind qCtagsSense::kindType( const QChar& c )
{
	if ( mCharKind.contains( c ) )
	{
		return mCharKind[ c ];
	}
	
	return qCtagsSense::Unknow;
}

QChar qCtagsSense::kindChar( qCtagsSense::Kind kind )
{
	return mKindChar.value( kind );
}

QString qCtagsSense::kindText( qCtagsSense::Kind kind )
{
	return mKindText.value( kind );
}

QString qCtagsSense::kindText( const QChar& c )
{
	return kindText( kindType( c ) );
}

void qCtagsSense::tagEntry( const QString& fileName )
{	
	if ( !mInitialized )
	{
		qWarning() << "qCtagsSense instance not initialized";
		return;
	}
	
	mIndexer->indexFile( fileName );
}

void qCtagsSense::tagEntries( const QMap<QString, QString>& entries )
{
	if ( !mInitialized )
	{
		qWarning() << "qCtagsSense instance not initialized";
		return;
	}
	
	mIndexer->indexBuffers( entries );
}
