#include "XUPProjectItemCache.h"
#include "XUPProjectItem.h"
#include "XUPProjectItemHelper.h"

#include <QDebug>

uint qHash( const XUPProjectItemCache::ProjectPointer& pointer )
{
	return qHash( pointer ? pointer->fileName() : QString::null );
}

// XUPProjectItemCacheBackend

XUPProjectItemCacheBackend::XUPProjectItemCacheBackend( XUPProjectItemCache* cache, const QString& separator )
{
	mCache = cache;
	mMultiLineSeparator = separator;
}

QString XUPProjectItemCacheBackend::multiLineSeparator() const
{
	return mMultiLineSeparator;
}

QString XUPProjectItemCacheBackend::guessedVariable( XUPProjectItem* project, XUPProjectItem* variableProject, const QString& variable ) const
{
	
	if ( !mCache ) {
		return QString::null;
	}
	
	XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
	
	const QString name = QString( variable ).replace( '$', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	
	// environment variable
	if ( variable.startsWith( "$(" ) ) {
		if ( name == "PWD" ) {
			return project->path();
		}
		else {
			return QString::fromLocal8Bit( qgetenv( name.toLocal8Bit().constData() ) );
		}
	}
	// cache variable
	else if ( variable.startsWith( "$$[" ) ) {
		if ( name == "PWD" ) {
			return variableProject->path();
		}
		else {
			return QString( cachedData.value( project ).value( name ) ).replace( mMultiLineSeparator, " " );
		}
	}
	
	return QString::null;
}

QString XUPProjectItemCacheBackend::guessedValue( XUPProjectItem* project, XUPProjectItem* valueProject, const QString& value ) const
{
	QRegExp rx( "(?:[^$]|^)(\\$[(\\[][\\w._ ]+[)\\]])" );
	QString guessed = value;
	int pos = 0;

	while ( ( pos = rx.indexIn( value, pos ) ) != -1 ) {
		guessed.replace( rx.cap( 1 ), guessedVariable( project, valueProject, rx.cap( 1 ) ) );
		pos += rx.matchedLength();
	}

	return guessed;
}

void XUPProjectItemCacheBackend::updateVariable( XUPProjectItem* project, const QString& variable, const QString& value, const QString& op )
{
	Q_UNUSED( op );
	
	if ( !mCache ) {
		return;
	}
	
	XUPProjectItemCache::ProjectCache& cachedData = mCache->cachedData();
	cachedData[ project ][ variable ] = value;
}

void XUPProjectItemCacheBackend::recursiveScan( XUPProjectItem* project, XUPItem* _root )
{
	XUPItem* root = _root ? _root : project;
	QStringList values;
	
	//qWarning() << project->fileName() << root->project()->rootIncludeProject()->fileName() << root->displayText();
	Q_ASSERT( project == root->project()->rootIncludeProject() );
	
	foreach ( XUPItem* child, root->childrenList() ) {
		switch ( child->type() ) {
			case XUPItem::Value:
			case XUPItem::File:
			case XUPItem::Path: {
				const QString content = guessedValue( project, child->project(), child->content() );
				child->setCacheValue( "content", content );
				values << content;
				break;
			}
			case XUPItem::DynamicFolder:
				continue;
			case XUPItem::Project:
				if ( project != child->project()->rootIncludeProject() ) {
					continue;
				}
				break;
			case XUPItem::Function: {
				const QString parameters = guessedValue( project, child->project(), child->attribute( "parameters" ) );
				child->setCacheValue( "parameters", parameters );
				break;
			}
			default:
				break;
		}
		
		recursiveScan( project, child );
	}
	
	if ( root->type() == XUPItem::Variable ) {
		updateVariable( project, root->attribute( "name" ), values.join( mMultiLineSeparator ), root->attribute( "operator" ) );
	}
	
	if ( root->type() != XUPItem::DynamicFolder ) {
		cacheRecursiveScanHook( project, root );
	}
}

bool XUPProjectItemCacheBackend::cacheRecursiveScanHook( XUPProjectItem* project, XUPItem* item )
{
    Q_UNUSED( project );
	Q_UNUSED( item );
    return false;
}

// XUPProjectItemCache

XUPProjectItemCache::ProjectCache& XUPProjectItemCache::cachedData() const
{
	return mCache;
}

void XUPProjectItemCache::build( XUPProjectItem* project )
{
//qWarning() << "*** BUILD" << project->fileName() << ( root ? root->displayText() : "" );
	clear( project );
	update( project, project );
}

void XUPProjectItemCache::update( XUPProjectItem* project, XUPItem* root )
{
//qWarning() << "*** UPDATE" << project->fileName() << ( root ? root->displayText() : "" );
	if ( project->cacheBackend() ) {
		project->cacheBackend()->recursiveScan( project, root );
	}
}

void XUPProjectItemCache::clear( XUPProjectItem* project )
{
	mCache.remove( project );
}

#ifndef QT_NO_DEBUG
void XUPProjectItemCache::debug( bool full ) const
{
	int indent = 0;
	
	foreach ( XUPProjectItem* project, mCache.keys() ) {
		qWarning() << QString( "%1Project cache - %2" ).arg( indentString( indent ) ).arg( project->fileName() ).toLocal8Bit();
		
		if ( full ) {
			indent++;
			
			foreach ( const QString& variable, mCache[ project ].keys() ) {
				qWarning() << QString( "%1%2" ).arg( indentString( indent ) ).arg( variable ).toLocal8Bit();
				
				indent++;
				
				foreach ( const QString& value, project->documentFilters().splitValue( mCache[ project ][ variable ] ) ) {
					qWarning() << QString( "%1%2" ).arg( indentString( indent ) ).arg( value ).toLocal8Bit();
				}
				
				indent--;
			}
			
			indent--;
		}
	}
}
#endif

QString XUPProjectItemCache::value( XUPProjectItem* project, const QString& variable ) const
{
	const XUPProjectItemCacheBackend* backend = project->cacheBackend();
	
	if ( !backend ) {
		return QString::null;
	}
	
	if ( !mCache.value( project ).contains( variable ) ) {
		const_cast<XUPProjectItemCache*>( this )->build( project );
	}
	
	return QString( mCache.value( project ).value( variable ) ).replace( backend->multiLineSeparator(), " " );
}

QStringList XUPProjectItemCache::values( XUPProjectItem* project, const QString& variable ) const
{
	const XUPProjectItemCacheBackend* backend = project->cacheBackend();
	
	if ( !backend ) {
		return QStringList();
	}
	
	if ( !mCache.value( project ).contains( variable ) ) {
		const_cast<XUPProjectItemCache*>( this )->build( project );
	}
	
	return mCache.value( project ).value( variable ).split( backend->multiLineSeparator() );
}

QString XUPProjectItemCache::evaluatedContent( XUPProjectItem* project, XUPProjectItem* valueProject, const QString& value ) const
{
	if ( project->cacheBackend() ) {
		return project->cacheBackend()->guessedValue( project, valueProject, value );
	}
	
	return QString::null;
}

QString XUPProjectItemCache::indentString( int indent ) const
{
	return QString( indent, '\t' );
}
