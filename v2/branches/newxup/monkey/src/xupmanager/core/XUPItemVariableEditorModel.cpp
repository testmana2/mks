#include "XUPItemVariableEditorModel.h"
#include "ProjectTypesIndex.h"
#include "XUPProjectItem.h"
#include "XUPProjectItemHelper.h"

#include <QApplication>
#include <QPalette>
#include <QFont>
#include <QDebug>

XUPItemVariableEditorModel::XUPItemVariableEditorModel( QObject* parent )
	: QAbstractItemModel( parent )
{
	mMode = XUPItemVariableEditorModel::Out;
	mRootItem = 0;
	mDocumentFilterMap = 0;
	mQuoteValues = false;
	mFriendlyDisplayText = false;
}

int XUPItemVariableEditorModel::columnCount( const QModelIndex& parent ) const
{
	XUPItemVariableEditorModelItem* item = this->item( parent );
	return item ? 1 : 0;
}

QVariant XUPItemVariableEditorModel::data( const QModelIndex& index, int role ) const
{
	XUPItemVariableEditorModelItem* item = this->item( index );
	
	if ( item && item != &mRoot ) {
		const XUPItemVariableEditorModelItem* parent = mParentMapping.value( item );
		const bool isVariable = index.isValid() && !index.parent().isValid();
		bool enabled = parent && parent != &mRoot ? parent->enabled : item->enabled;
		
		if ( enabled ) {
			enabled = item->enabled;
		}
		
		switch ( role ) {
			case Qt::DecorationRole: {
				QString iconFilePath;
				
				if ( isVariable && mFriendlyDisplayText && mDocumentFilterMap ) {
					iconFilePath = mDocumentFilterMap->variableDisplayIcon( item->string );
				}
				
				return iconFilePath.isEmpty() ? QVariant() : QIcon( iconFilePath );
			}
			case Qt::DisplayRole:
			case Qt::ToolTipRole:
			case Qt::EditRole: {
				if ( isVariable && mFriendlyDisplayText && mDocumentFilterMap ) {
					return mDocumentFilterMap->variableDisplayText( item->string );
				}
				
				return item->string;
			}
			case Qt::CheckStateRole:
				return enabled ? Qt::Checked : Qt::Unchecked;
			case Qt::ForegroundRole:
				return enabled ? QVariant() : QApplication::palette().brush( QPalette::Disabled, QPalette::WindowText );
			case Qt::FontRole: {
				QFont font;
				font.setStrikeOut( !enabled );
				return font;
			}
		}
	}
	
	return QVariant();
}

QModelIndex XUPItemVariableEditorModel::index( int row, int column, const QModelIndex& parent ) const
{
	if ( column != 0 ) {
		return QModelIndex();
	}
	
	XUPItemVariableEditorModelItem* item = this->item( parent );
	return item && row < item->children.count() ? createIndex( row, column, item ) : QModelIndex();
}

QModelIndex XUPItemVariableEditorModel::parent( const QModelIndex& index ) const
{
	XUPItemVariableEditorModelItem* parentItem = static_cast<XUPItemVariableEditorModelItem*>( index.internalPointer() );
	XUPItemVariableEditorModelItem* parentItemParent = mParentMapping.value( parentItem );
	return parentItemParent ? createIndex( parentItemParent->children.indexOf( *parentItem ), 0, parentItemParent ) : QModelIndex();
}

int	XUPItemVariableEditorModel::rowCount( const QModelIndex& parent ) const
{
	XUPItemVariableEditorModelItem* item = this->item( parent );
	return item ? item->children.count() : 0;
}

Qt::ItemFlags XUPItemVariableEditorModel::flags( const QModelIndex& index ) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags( index );
	
	if ( index.isValid() ) {
		flags |= Qt::ItemIsUserCheckable;
	}
	
	return flags;
}

bool XUPItemVariableEditorModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	if ( !index.isValid() ) {
		return false;
	}
	
	XUPItemVariableEditorModelItem* item = this->item( index );
	
	switch ( role ) {
		case Qt::EditRole:
		case Qt::DisplayRole: {
			const bool isVariable = index.parent() == QModelIndex();
			const QString string = isVariable ? value.toString() : quotedValue( value.toString() );
			const QModelIndex cIndex = childIndex( index.parent(), string );
			
			if ( cIndex.isValid() && cIndex != index ) {
				return false;
			}
			
			switch ( mMode ) {
				case XUPItemVariableEditorModel::Out: {
					if ( isVariable && mFilteredVariables.contains( string ) ) {
						return false;
					}
					break;
				}
				case XUPItemVariableEditorModel::In: {
					if ( isVariable && !mFilteredVariables.contains( string ) ) {
						return false;
					}
					break;
				}
			}
			
			item->string = string;
			break;
		}
		case Qt::CheckStateRole:
			item->enabled = value.toInt() == Qt::Checked;
			
			if ( hasChildren( index ) ) {
				emit dataChanged( index.child( 0, 0 ), index.child( rowCount( index ) -1, columnCount( index ) -1 ) );
			}
			
			break;
		default:
			return false;
	}
	
	emit dataChanged( index, index );
	return true;
}

XUPItemVariableEditorModelItem* XUPItemVariableEditorModel::item( const QModelIndex& index ) const
{
	XUPItemVariableEditorModelItem* parentItem = static_cast<XUPItemVariableEditorModelItem*>( index.internalPointer() );
	XUPItemVariableEditorModelItem* item = parentItem && !parentItem->children.isEmpty() ? &( parentItem->children[ index.row() ] ) : 0;
	return index == QModelIndex() ? &mRoot : item;
}

QModelIndex XUPItemVariableEditorModel::childIndex( const QModelIndex& index, const QString& _string ) const
{
	const QString string = index.parent() == QModelIndex() ? _string : quotedValue( string );
	XUPItemVariableEditorModelItem* item = this->item( index );
	
	if ( item ) {
		for ( int i = 0; i < item->children.count(); i++ ) {
			XUPItemVariableEditorModelItem& it = item->children[ i ];
			
			if ( it.string == string ) {
				return createIndex( i, 0, item );
			}
		}
	}
	
	return QModelIndex();
}

void XUPItemVariableEditorModel::clear()
{
	const int count = mRoot.children.count();
	
	if ( count > 0 ) {
		beginRemoveRows( QModelIndex(), 0, count -1 );
		mRoot = XUPItemVariableEditorModelItem();
		mParentMapping.clear();
		mRootItem = 0;
		mDocumentFilterMap = 0;
		endRemoveRows();
	}
}

void XUPItemVariableEditorModel::setRootItem( const XUPItemVariableEditorModelItem& item )
{
	setRootItemInternal( item, true );
}

void XUPItemVariableEditorModel::setRootItem( XUPItem* item )
{
	revert( item );
}

QModelIndex XUPItemVariableEditorModel::addVariable( const QString& variable )
{
	switch ( mMode ) {
		case XUPItemVariableEditorModel::Out: {
			if ( mFilteredVariables.contains( variable ) ) {
				return QModelIndex();
			}
			break;
		}
		case XUPItemVariableEditorModel::In: {
			if ( !mFilteredVariables.contains( variable ) ) {
				return QModelIndex();
			}
			break;
		}
	}
	
	return addValue( QModelIndex(), variable );
}

QModelIndex XUPItemVariableEditorModel::addValue( const QModelIndex& variable, const QString& _value )
{
	const QString value = variable == QModelIndex() ? _value : quotedValue( _value );
	const QModelIndex childIndex = this->childIndex( variable, value );
	
	if ( childIndex.isValid() || value.isEmpty() ) {
		return childIndex;
	}
	
	XUPItemVariableEditorModelItem* item = this->item( variable );
	
	if ( item ) {
		const int count = item->children.count();
		beginInsertRows( variable, count, count );
		item->children << XUPItemVariableEditorModelItem( value, true, 0 );
		mParentMapping[ &( item->children.last() ) ] = item;
		endInsertRows();
		return createIndex( item->children.count() -1, 0, item );
	}
	
	return QModelIndex();
}

QStringList XUPItemVariableEditorModel::fileVariables() const
{
	return mDocumentFilterMap ? mDocumentFilterMap->fileVariables() : QStringList();
}

QStringList XUPItemVariableEditorModel::pathVariables() const
{
	return mDocumentFilterMap ? mDocumentFilterMap->pathVariables() : QStringList();
}

QStringList XUPItemVariableEditorModel::knownVariables() const
{
	return mDocumentFilterMap ? mDocumentFilterMap->knownVariables() : QStringList();
}

void XUPItemVariableEditorModel::setFilterMode( XUPItemVariableEditorModel::FilterMode mode )
{
	mMode = mode;
}

XUPItemVariableEditorModel::FilterMode XUPItemVariableEditorModel::mode() const
{
	return mMode;
}

void XUPItemVariableEditorModel::setFilteredVariables( const QStringList& filters )
{
	mFilteredVariables = filters;
}

QStringList XUPItemVariableEditorModel::filteredVariables() const
{
	return mFilteredVariables;
}

void XUPItemVariableEditorModel::setFriendlyDisplayText( bool friendly )
{
	mFriendlyDisplayText = friendly;
	emit dataChanged( index( 0, 0 ), index( rowCount() -1, columnCount() -1 ) );
}

bool XUPItemVariableEditorModel::friendlyDisplayText() const
{
	return mFriendlyDisplayText;
}

void XUPItemVariableEditorModel::setQuoteString( const QString& string )
{
	mQuoteString = string;
}

QString XUPItemVariableEditorModel::quoteString() const
{
	return mQuoteString;
}

void XUPItemVariableEditorModel::setQuoteValues( bool quote )
{
	mQuoteValues = quote;
}

bool XUPItemVariableEditorModel::quoteValues() const
{
	return mQuoteValues;
}

QString XUPItemVariableEditorModel::quotedValue( const QString& value ) const
{
	if ( mQuoteValues ) {
		if ( value.contains( " " ) && ( !value.startsWith( mQuoteString ) && !value.endsWith( mQuoteString ) ) ) {
			return QString( value ).prepend( mQuoteString ).append( mQuoteString );
		}
	}
	
	return value;
}

void XUPItemVariableEditorModel::revert( XUPItem* item )
{
	clear();
	
	if ( !item ) {
		return;
	}
	
	const XUPProjectItem* project = item->project();
	mRootItem = item;
	mDocumentFilterMap = &project->documentFilters();
	XUPItemVariableEditorModelItem root;
	
	// only retreives values of variables that are direct children of item
	foreach ( XUPItem* variable, mRootItem->childrenList() ) {
		if ( variable->type() == XUPItem::Variable ) {
			XUPItemVariableEditorModelItem si( variable->attribute( "name" ), true, variable );
			
			switch ( mMode ) {
				case XUPItemVariableEditorModel::Out: {
					if ( si.string.startsWith( XUPProjectItemHelper::SettingsScopeName ) || mFilteredVariables.contains( si.string ) ) {
						continue;
					}
					break;
				}
				case XUPItemVariableEditorModel::In: {
					if ( si.string.startsWith( XUPProjectItemHelper::SettingsScopeName ) || !mFilteredVariables.contains( si.string ) ) {
						continue;
					}
					break;
				}
			}
			
			foreach ( XUPItem* value, variable->childrenList() ) {
				switch ( value->type() ) {
					case XUPItem::Value:
					case XUPItem::File:
					case XUPItem::Path:
						si.children << XUPItemVariableEditorModelItem( value->content(), true, value );
						break;
					default:
						continue;
				}
			}
			
			root.children << si;
		}
	}
	
	setRootItemInternal( root, false );
}

void XUPItemVariableEditorModel::revert()
{
	revert( mRootItem );
}

bool XUPItemVariableEditorModel::submit()
{
	const bool senderIsItemSelectionModel = sender() ? sender()->inherits( "QItemSelectionModel" ) : false;
	
	if ( !mRootItem || senderIsItemSelectionModel ) {
		return false;
	}
	
	foreach ( const XUPItemVariableEditorModelItem& variable, mRoot.children ) {
		const bool isFileVariable = fileVariables().contains( variable.string );
		const bool isPathVariable = pathVariables().contains( variable.string );
		XUPItem* variableItem = variable.item;
		
		if ( !variable.enabled ) {
			if ( variableItem ) {
				variableItem->parent()->removeChild( variableItem );
			}
			
			continue;
		}
		
		if ( !variableItem ) {
			variableItem = mRootItem->addChild( XUPItem::Variable );
			variableItem->setAttribute( "name", variable.string );
		}
		
		foreach ( const XUPItemVariableEditorModelItem& value, variable.children ) {
			XUPItem* valueItem = value.item;
			
			if ( value.enabled ) {
				if ( !valueItem ) {
					const XUPItem::Type type = isFileVariable ? XUPItem::File : ( isPathVariable ? XUPItem::Path : XUPItem::Value );
					valueItem = variableItem->addChild( type );
				}
				
				valueItem->setContent( value.string );
			}
			else {
				if ( valueItem ) {
					valueItem->parent()->removeChild( valueItem );
				}
			}
		}
		
		if ( !variableItem->hasChildren() ) {
			variableItem->parent()->removeChild( variableItem );
		}
	}
	
	revert();
	
	return true;
}

void XUPItemVariableEditorModel::buildParentMapping( XUPItemVariableEditorModelItem& item )
{
	if ( item == mRoot ) {
		mParentMapping[ &mRoot ] = 0;
	}
	
	for ( int i = 0; i < item.children.count(); i++ ) {
		XUPItemVariableEditorModelItem& child = item.children[ i ];
		mParentMapping[ &child ] = &item;
		buildParentMapping( child );
	}
}

void XUPItemVariableEditorModel::setRootItemInternal( const XUPItemVariableEditorModelItem& item, bool clearModel )
{
	if ( clearModel ) {
		clear();
	}
	
	if ( item == XUPItemVariableEditorModelItem() ) {
		return;
	}
	
	const int count = item.children.count();
	
	beginInsertRows( QModelIndex(), 0, count -1 );
	mRoot = item;
	mRoot.enabled = true;
	buildParentMapping( mRoot );
	endInsertRows();
}
