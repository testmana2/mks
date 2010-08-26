#include "XUPDynamicFolderItem.h"
#include "XUPProjectItem.h"
#include "XUPProjectModel.h"

#include <QFileSystemModel>
#include <QDebug>

class pFileSystemModel : public QFileSystemModel
{
	friend class XUPDynamicFolderItem;
	
public:
	pFileSystemModel( QObject* parent = 0 )
		: QFileSystemModel( parent )
	{
	}
};

class XUPDynamicFolderChildItem : public XUPItem
{
	friend class XUPDynamicFolderItem;
	
public:
	virtual ~XUPDynamicFolderChildItem()
	{
		// remove children
		if ( mFSModel->hasChildren( mFSIndex ) ) {
			for ( int row = 0; row < mFSModel->rowCount( mFSIndex ); row++ ) {
				const QModelIndex index = mFSModel->index( row, 0, mFSIndex );
				delete mDynamicFolderItem->mFSItems.value( index );
			}
		}
		
		// unregister
		//qWarning() << "** UNREGISTERING" << this << mFSModel->filePath( mFSIndex );
		mDynamicFolderItem->mFSItems.remove( mFSIndex );
	}
	
	virtual int childCount() const
	{
		if ( mFSModel->canFetchMore( mFSIndex ) ) {
			mFSModel->fetchMore( mFSIndex );
		}
		
		return mFSModel->rowCount( mFSIndex );
	}

	virtual bool hasChildren() const
	{
		return mFSModel->hasChildren( mFSIndex );
	}
	
	virtual int childIndex( XUPItem* child ) const
	{
		XUPDynamicFolderChildItem* item = static_cast<XUPDynamicFolderChildItem*>( child );
		return item->mFSIndex.row();
	}
	
	virtual XUPItem* child( int row )
	{
		const QPersistentModelIndex index = mFSIndex.child( row, 0 );
		
		if ( mDynamicFolderItem->mFSItems.contains( index ) ) {
			return mDynamicFolderItem->mFSItems.value( index );
		}

		if ( index.isValid() ) {
			XUPItem* childItem = new XUPDynamicFolderChildItem( mDynamicFolderItem, index, const_cast<XUPDynamicFolderChildItem*>( this ) );
			return childItem;
		}
		
		return 0;
	}
	
	virtual XUPItemList childrenList() const
	{
		XUPItemList children;
		
		if ( hasChildren() ) {
			for ( int row = 0; row < childCount(); row++ ) {
				const QModelIndex index = mFSModel->index( row, 0, mFSIndex );
				XUPItem* item = mDynamicFolderItem->mFSItems.value( index );
				
				if ( item ) {
					children << item;
				}
			}
		}
		
		return children;
	}
	
	virtual XUPItem::Type type() const
	{
		if ( !mFSIndex.isValid() ) {
			return XUPItem::Unknow;
		}
		
		return mFSModel->isDir( mFSIndex ) ? XUPItem::Folder : XUPItem::File;
	}
	
	virtual QString displayText() const
	{
		return mFSIndex.data( Qt::DisplayRole ).toString();
	}
	
	virtual QIcon displayIcon() const
	{
		return mFSIndex.data( Qt::DecorationRole ).value<QIcon>();
	}
	
	virtual QString attribute( const QString& name, const QString& defaultValue = QString::null ) const
	{
		if ( name.compare( "name", Qt::CaseInsensitive ) == 0 ) {
			switch ( type() ) {
				case XUPItem::Folder:
					return mFSModel->filePath( mFSIndex );
				default:
					break;
			}
		}
		
		return XUPItem::attribute( name, defaultValue );
	}
	
	virtual QString content() const
	{
		switch ( type() ) {
			case XUPItem::File:
				return mFSModel->filePath( mFSIndex );
			default:
				break;
		}
		
		return XUPItem::content();
	}

protected:
	XUPDynamicFolderItem* mDynamicFolderItem;
	pFileSystemModel* mFSModel;
	QPersistentModelIndex mFSIndex;
	
	XUPDynamicFolderChildItem( XUPDynamicFolderItem* dynamicFolderItem, const QPersistentModelIndex& fsIndex, XUPItem* parent )
		: XUPItem( QDomElement(), parent )
	{
		Q_ASSERT( dynamicFolderItem );
		Q_ASSERT( dynamicFolderItem->mFSModel );
		Q_ASSERT( fsIndex.isValid() );
		
		mDynamicFolderItem = dynamicFolderItem;
		mFSModel = mDynamicFolderItem->mFSModel;
		mFSIndex = fsIndex;
		
		mDynamicFolderItem->mFSItems[ mFSIndex ] = this;
		
		//qWarning() << "** REGISTERED IN" << this << mFSModel->filePath( fsIndex ) << parent->displayText();
	}
};

XUPDynamicFolderItem::XUPDynamicFolderItem( const QDomElement& node, XUPItem* parent )
	: QObject( parent->project() ), XUPItem( node, parent )
{
	mFSModel = new pFileSystemModel( this );
	
	connect( mFSModel, SIGNAL( columnsAboutToBeInserted( const QModelIndex&, int, int ) ), this, SLOT( columnsAboutToBeInserted( const QModelIndex&, int, int ) ) );
	connect( mFSModel, SIGNAL( columnsAboutToBeMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ), this, SLOT( columnsAboutToBeMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ) );
	connect( mFSModel, SIGNAL( columnsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( columnsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
	connect( mFSModel, SIGNAL( columnsInserted( const QModelIndex&, int, int ) ), this, SLOT( columnsInserted( const QModelIndex&, int, int ) ) );
	connect( mFSModel, SIGNAL( columnsMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ), this, SLOT( columnsMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ) );
	connect( mFSModel, SIGNAL( columnsRemoved( const QModelIndex&, int, int ) ), this, SLOT( columnsRemoved( const QModelIndex&, int, int ) ) );
	connect( mFSModel, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( dataChanged( const QModelIndex&, const QModelIndex& ) ) );
	connect( mFSModel, SIGNAL( headerDataChanged( Qt::Orientation, int, int ) ), this, SLOT( headerDataChanged( Qt::Orientation, int, int ) ) );
	connect( mFSModel, SIGNAL( layoutAboutToBeChanged() ), this, SLOT( layoutAboutToBeChanged() ) );
	connect( mFSModel, SIGNAL( layoutChanged() ), this, SLOT( layoutChanged() ) );
	connect( mFSModel, SIGNAL( modelAboutToBeReset() ), this, SLOT( modelAboutToBeReset() ) );
	connect( mFSModel, SIGNAL( modelReset() ), this, SLOT( modelReset() ) );
	connect( mFSModel, SIGNAL( rowsAboutToBeInserted( const QModelIndex&, int, int ) ), this, SLOT( rowsAboutToBeInserted( const QModelIndex&, int, int ) ) );
	connect( mFSModel, SIGNAL( rowsAboutToBeMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ), this, SLOT( rowsAboutToBeMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ) );
	connect( mFSModel, SIGNAL( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ), this, SLOT( rowsAboutToBeRemoved( const QModelIndex&, int, int ) ) );
	connect( mFSModel, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SLOT( rowsInserted( const QModelIndex&, int, int ) ) );
	connect( mFSModel, SIGNAL( rowsMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ), this, SLOT( rowsMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ) );
	connect( mFSModel, SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ), this, SLOT( rowsRemoved( const QModelIndex&, int, int ) ) );
	connect( mFSModel, SIGNAL( rootPathChanged( const QString& ) ), this, SLOT( rootPathChanged( const QString& ) ) );
	
	mFSRootIndex = mFSModel->setRootPath( node.attribute( "root" ) );
}

XUPDynamicFolderItem::~XUPDynamicFolderItem()
{
}

int XUPDynamicFolderItem::childCount() const
{
	if ( mFSModel->canFetchMore( mFSRootIndex ) ) {
		mFSModel->fetchMore( mFSRootIndex );
	}
	
	return mFSModel->rowCount( mFSRootIndex );
}

bool XUPDynamicFolderItem::hasChildren() const
{
	return mFSModel->hasChildren( mFSRootIndex );
}

int XUPDynamicFolderItem::childIndex( XUPItem* child ) const
{
	XUPDynamicFolderChildItem* item = static_cast<XUPDynamicFolderChildItem*>( child );
	return item->mFSIndex.parent() == mFSRootIndex ? item->mFSIndex.row() : -1;
}

XUPItem* XUPDynamicFolderItem::child( int row )
{
	const QPersistentModelIndex index = mFSModel->index( row, 0, mFSRootIndex );
	
	if ( mFSItems.contains( index ) ) {
		return mFSItems.value( index );
	}

	if ( index.isValid() ) {
		XUPItem* childItem = new XUPDynamicFolderChildItem( this, index, this );
		return childItem;
	}
	
	return 0;
}

XUPItemList XUPDynamicFolderItem::childrenList() const
{
	XUPItemList children;
	
	if ( hasChildren() ) {
		for ( int row = 0; row < childCount(); row++ ) {
			const QModelIndex index = mFSModel->index( row, 0, mFSRootIndex );
			XUPItem* item = mFSItems.value( index );
			
			if ( item ) {
				children << item;
			}
		}
	}
	
	return children;
}

void XUPDynamicFolderItem::columnsAboutToBeInserted( const QModelIndex& parent, int start, int end )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::columnsAboutToBeMoved( const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationColumn )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::columnsAboutToBeRemoved( const QModelIndex& parent, int start, int end )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::columnsInserted( const QModelIndex& parent, int start, int end )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::columnsMoved( const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationColumn )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::columnsRemoved( const QModelIndex& parent, int start, int end )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::dataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::headerDataChanged( Qt::Orientation orientation, int first, int last )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::layoutAboutToBeChanged()
{
	XUPProjectModel* model = this->model();
	
	if ( model ) {
		emit model->layoutAboutToBeChanged();
	}
}

void XUPDynamicFolderItem::layoutChanged()
{
	XUPProjectModel* model = this->model();
	
	if ( model ) {
		emit model->layoutChanged();
	}
}

void XUPDynamicFolderItem::modelAboutToBeReset()
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::modelReset()
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::rowsAboutToBeInserted( const QModelIndex& parent, int start, int end )
{
	//qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::rowsAboutToBeMoved( const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::rowsAboutToBeRemoved( const QModelIndex& parent, int start, int end )
{
	XUPItem* item = mFSItems.value( parent, this );
	XUPProjectModel* model = this->model();
	
	// Don't handle mFSRootIndex parents - parents will be removed, so tweak the parent / start / end to remove
	if ( mFSModel->filePath( parent ).length() < mFSModel->filePath( mFSRootIndex ).length() || !mFSRootIndex.isValid() ) {
		item = this;
		start = 0;
		end = mFSModel->rowCount( mFSRootIndex );
	}
	
	Q_ASSERT( item );
	
	if ( model ) {
		model->beginRemoveRows( item->index(), start, end );
		
		for ( int row = start; row <= end; row++ ) {
			const QModelIndex index = mFSModel->index( row, 0, parent );
			delete mFSItems.value( index );
		}
	}
}

void XUPDynamicFolderItem::rowsInserted( const QModelIndex& parent, int start, int end )
{
	// Don't handle mFSRootIndex parents
	if ( mFSModel->filePath( parent ).length() < mFSModel->filePath( mFSRootIndex ).length() || !mFSRootIndex.isValid() ) {
		return;
	}
	
	XUPItem* item = mFSItems.value( parent, this );
	XUPProjectModel* model = this->model();
	
	Q_ASSERT( item );
	
	if ( model ) {
		model->beginInsertRows( item->index(), start, end );
		// nothing special to do as we do direct mapping
		model->endInsertRows();
	}
}

void XUPDynamicFolderItem::rowsMoved( const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow )
{
	qWarning() << Q_FUNC_INFO;
}

void XUPDynamicFolderItem::rowsRemoved( const QModelIndex& parent, int start, int end )
{
	XUPProjectModel* model = this->model();
	
	if ( model ) {
		model->endRemoveRows();
	}
}

void XUPDynamicFolderItem::rootPathChanged( const QString& newPath )
{
	qWarning() << Q_FUNC_INFO;
}
