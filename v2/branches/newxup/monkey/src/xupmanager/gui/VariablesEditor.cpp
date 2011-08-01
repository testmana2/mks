#include "VariablesEditor.h"
#include "xupmanager/core/XUPProjectItem.h"
#include "xupmanager/core/XUPProjectItemHelper.h"
#include "pMonkeyStudio.h"

#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

struct StringItem
{
	typedef QList<StringItem> List;
	
	StringItem( const QString& string = QString::null, bool enabled = false, XUPItem* item = 0 )
	{
		this->string = string;
		this->enabled = enabled;
		this->item = item;
	}
	
	bool operator<( const StringItem& other ) const
	{
		return string < other.string;
	}
	
	bool operator==( const StringItem& other ) const
	{
		return string == other.string
			&& enabled == other.enabled
			&& item == other.item
			&& children == other.children
			;
	}
	
	QString string;
	bool enabled;
	XUPItem* item;
	StringItem::List children;
};

class StringItemModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	StringItemModel( QObject* parent = 0 )
		: QAbstractItemModel( parent )
	{
	}
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const
	{
		StringItem* item = this->item( parent );
		return item ? 1 : 0;
	}
	
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const
	{
		StringItem* item = this->item( index );
		
		if ( item && item != &mRoot ) {
			switch ( role ) {
				case Qt::DisplayRole:
				case Qt::EditRole:
					return item->string;
				case Qt::CheckStateRole:
					return item->enabled ? Qt::Checked : Qt::Unchecked;
			}
		}
		
		return QVariant();
	}
	
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const
	{
		if ( column != 0 ) {
			return QModelIndex();
		}
		
		StringItem* item = this->item( parent );
		return item ? createIndex( row, column, item ) : QModelIndex();
	}
	
	virtual QModelIndex parent( const QModelIndex& index ) const
	{
		StringItem* parentItem = static_cast<StringItem*>( index.internalPointer() );
		StringItem* parentItemParent = mMapping.value( parentItem );
		return parentItemParent ? createIndex( parentItemParent->children.indexOf( *parentItem ), 0, parentItemParent ) : QModelIndex();
	}
	
	virtual int	rowCount( const QModelIndex& parent = QModelIndex() ) const
	{
		StringItem* item = this->item( parent );
		return item ? item->children.count() : 0;
	}
	
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const
	{
		Qt::ItemFlags flags = QAbstractItemModel::flags( index );
		
		if ( index.isValid() ) {
			flags |= Qt::ItemIsUserCheckable;
		}
		
		return flags;
	}
	
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole )
	{
		if ( !index.isValid() ) {
			return false;
		}
		
		StringItem* item = this->item( index );
		
		switch ( role ) {
			case Qt::EditRole:
			case Qt::DisplayRole:
				item->string = value.toString();
				break;
			case Qt::CheckStateRole:
				item->enabled = value.toInt() == Qt::Checked;
				break;
			default:
				return false;
		}
		
		emit dataChanged( index, index );
		return true;
	}
	
	StringItem* item( const QModelIndex& index ) const
	{
		StringItem* parentItem = static_cast<StringItem*>( index.internalPointer() );
		StringItem* item = parentItem ? &( parentItem->children[ index.row() ] ) : 0;
		return index == QModelIndex() ? &mRoot : item;
	}
	
	void clear()
	{
		const int count = mRoot.children.count();
		
		if ( count > 0 ) {
			beginRemoveRows( QModelIndex(), 0, count -1 );
			mRoot = StringItem();
			mMapping.clear();
			endRemoveRows();
		}
	}
	
	void setRootItem( const StringItem& item )
	{
		clear();
		
		if ( item == StringItem() ) {
			return;
		}
		
		const int count = item.children.count();
		
		beginInsertRows( QModelIndex(), 0, count -1 );
		mRoot = item;
		buildParentMapping( mRoot );
		endInsertRows();
	}
	
	QModelIndex addChild( const QModelIndex& index, const StringItem& child )
	{
		StringItem* item = this->item( index );
		
		if ( item ) {
			foreach ( const StringItem& it, item->children ) {
				if ( it.string == child.string ) {
					return QModelIndex();
				}
			}
		
			const int count = item->children.count();
			beginInsertRows( index, count, count );
			item->children << child;
			mMapping[ &( item->children.last() ) ] = item;
			endInsertRows();
			return createIndex( item->children.count() -1, 0, item );
		}
		
		return QModelIndex();
	}

protected:
	mutable StringItem mRoot;
	QHash<StringItem*, StringItem*> mMapping;
	
	void buildParentMapping( StringItem& item )
	{
		if ( item == mRoot ) {
			mMapping[ &mRoot ] = 0;
		}
		
		for ( int i = 0; i < item.children.count(); i++ ) {
			StringItem* child = &( item.children[ i ] );
			mMapping[ child ] = &item;
			buildParentMapping( *child );
		}
	}
};

VariablesEditor::VariablesEditor( QWidget* parent )
	: QFrame( parent )
{
	mProject = 0;
	mModel = new StringItemModel( this );
	
	setupUi( this );
	lvVariables->setModel( mModel );
	lvValues->setModel( mModel );
	
	// tbOthersValuesAdd actions
	QMenu* addMenu = new QMenu( tbOthersValuesAdd );
	aOthersValuesAddValue = addMenu->addAction( tr( "As Value..." ) );
	aOthersValuesAddFile = addMenu->addAction( tr( "As File..." ) );
	aOthersValuesAddPath = addMenu->addAction( tr( "As Path..." ) );
	tbOthersValuesAdd->setMenu( addMenu );
	
	// tbOthersValuesEdit actions
	QMenu* editMenu = new QMenu( tbOthersValuesEdit );
	aOthersValuesEditValue = editMenu->addAction( tr( "As Value..." ) );
	aOthersValuesEditFile = editMenu->addAction( tr( "As File..." ) );
	aOthersValuesEditPath = editMenu->addAction( tr( "As Path..." ) );
	tbOthersValuesEdit->setMenu( editMenu );
	
	connect( lvVariables->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvVariables_selectionModel_selectionChanged() ) );
}

VariablesEditor::~VariablesEditor()
{
}

void VariablesEditor::init( XUPProjectItem* project )
{
	const DocumentFilterMap& filters = project->documentFilters();
	mProject = project;
	mFileVariables = filters.fileVariables();
	mPathVariables = filters.pathVariables();
	mManagedVariables = QStringList( mFileVariables );
	StringItem root;
	
	// only load values from root variables
	foreach ( XUPItem* variable, mProject->childrenList() ) {
		if ( variable->type() == XUPItem::Variable ) {
			StringItem si( variable->attribute( "name" ), true, variable );
			
			if ( si.string.startsWith( XUPProjectItemHelper::SettingsScopeName ) || !mManagedVariables.contains( si.string ) ) {
				continue;
			}
			
			foreach ( XUPItem* value, variable->childrenList() ) {
				switch ( value->type() ) {
					case XUPItem::Value:
					case XUPItem::File:
					case XUPItem::Path:
						si.children << StringItem( value->content(), true, value );
						break;
					default:
						continue;
				}
			}
			
			root.children << si;
		}
	}
	
	mModel->setRootItem( root );
	
	const QModelIndex index = mModel->index( 0, 0 );
	lvVariables->setCurrentIndex( index );
	lvVariables->scrollTo( index );
}

void VariablesEditor::finalize()
{
	/*const DocumentFilterMap& filters = mProject->documentFilters();
	QListWidgetItem* curItem = lwOthersVariables->currentItem();
	on_lwOthersVariables_currentItemChanged( curItem, curItem );
	
	// tell about variables to remove
	foreach ( const QString& variable, mVariablesToRemove )
	{
		mValues[ variable ] = QString::null;
	}
	
	// update project
	foreach ( const QString& variable, mValues.keys() )
	{
		bool isEmpty = mValues[ variable ].trimmed().isEmpty();
		XUPItem* variableItem = this->variableItem( variable, !isEmpty );
		if ( !variableItem )
		{
			continue;
		}
		
		if ( !isEmpty )
		{
		#warning this part has to be rewrite to use XUPProjectItem::addFiles()
			if ( mFileVariables.contains( variable ) || mPathVariables.contains( variable ) )
			{
				// get child type
				XUPItem::Type type = mFileVariables.contains( variable ) ? XUPItem::File : XUPItem::Path;
				// get values
				QStringList values = filters.splitValue( mValues[ variable ] );
				
				// remove all child
				foreach ( XUPItem* child, variableItem->childrenList() )
				{
					if ( child->type() == type )
					{
						QString value = child->content();
						if ( values.contains( value ) )
						{
							values.removeAll( value );
						}
						else if ( !values.contains( value ) )
						{
							variableItem->removeChild( child );
						}
					}
				}
				
				// add new ones
				foreach ( const QString& v, values )
				{
					XUPItem* value = variableItem->addChild( type );
					value->setContent( v );
				}
			}
			/else if ( variable == "CONFIG" )
			{
				// update variable
				variableItem->setAttribute( "operator", "+=" );
				
				// remove all child values
				foreach ( XUPItem* child, variableItem->childrenList() )
				{
					if ( child->type() == XUPItem::Value )
					{
						variableItem->removeChild( child );
					}
				}
				
				// add new one
				XUPItem* value = variableItem->addChild( XUPItem::Value );
				value->setContent( mValues[ variable ] );
			}/
			else
			{
				// remove all child values
				foreach ( XUPItem* child, variableItem->childrenList() )
				{
					if ( child->type() == XUPItem::Value )
					{
						variableItem->removeChild( child );
					}
				}
				
				// add new one
				XUPItem* value = variableItem->addChild( XUPItem::Value );
				value->setContent( mValues[ variable ] );
			}
		}
		else if ( isEmpty && variableItem && variableItem->childCount() > 0 )
		{
			// remove all child values
			foreach ( XUPItem* child, variableItem->childrenList() )
			{
				if ( child->type() == XUPItem::Value || child->type() == XUPItem::File || child->type() == XUPItem::Path )
				{
					variableItem->removeChild( child );
				}
			}
		}
		
		// remove empty variable
		if ( variableItem->childCount() == 0 )
		{
			variableItem->parent()->removeChild( variableItem );
		}
	}*/
}

XUPItem* VariablesEditor::variableItem( const QString& variableName, bool create )
{
	XUPItem* variableItem = mProject->getVariables( mProject, variableName, false, 0 ).value( 0 );
	
	// create it if needed
	if ( !variableItem && create ) {
		variableItem = mProject->addChild( XUPItem::Variable );
		variableItem->setAttribute( "name", variableName );
	}
	
	// return item
	return variableItem;
}

void VariablesEditor::lvVariables_selectionModel_selectionChanged()
{
	const QModelIndex index = lvVariables->selectionModel()->selectedIndexes().value( 0 );
	tbOthersVariablesEdit->setEnabled( index.isValid() );
	gbOthersValues->setEnabled( index.isValid() );
	lvValues->setRootIndex( index );
}

void VariablesEditor::on_tbOthersVariablesAdd_clicked()
{
	const DocumentFilterMap& filters = mProject->documentFilters();
	const QStringList variables = filters.knownVariables();
	bool ok;
	
	const QString variable = QInputDialog::getItem( window(), tr( "Add variable..." ), tr( "Select a variable name or enter a new one" ), variables, 0, true, &ok );
	
	if ( !variable.isEmpty() && ok ) {
		const QModelIndex index = mManagedVariables.contains( variable ) ? QModelIndex() : mModel->addChild( QModelIndex(), StringItem( variable, true ) );
		
		if ( index.isValid() ) {
			lvVariables->setCurrentIndex( index );
			lvVariables->scrollTo( index );
		}
		else {
			QMessageBox::information( window(), tr( "Information..." ), tr( "This variable already exists or is filtered out." ) );
		}
	}
}

void VariablesEditor::on_tbOthersVariablesEdit_clicked()
{
	/*QListWidgetItem* item = lwOthersVariables->currentItem();
	
	if ( !item )
	{
		return;
	}
	
	bool ok;
	QString oldVariable = item->text();
	QString variable = QInputDialog::getText( window(), tr( "Edit variable..." ), tr( "Enter a new name for this variable" ), QLineEdit::Normal, oldVariable, &ok );
	
	if ( !variable.isEmpty() && ok )
	{
		if ( !mValues.keys().contains( variable ) && !mManagedVariables.contains( variable ) )
		{
			item->setText( variable );
			
			mValues.remove( oldVariable );
			if ( !mVariablesToRemove.contains( oldVariable ) )
			{
				mVariablesToRemove << oldVariable;
			}
		}
		else
		{
			QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This variable already exists or is filtered out." ) );
		}
	}*/
}

void VariablesEditor::on_tbOthersValuesAdd_clicked()
{
	on_tbOthersValuesAdd_triggered( aOthersValuesAddValue );
}

void VariablesEditor::on_tbOthersValuesAdd_triggered( QAction* action )
{
	/*QListWidgetItem* variableItem = lwOthersVariables->currentItem();
	
	if ( variableItem )
	{
		const QString title = tr( "Add a value..." );
		bool ok = true;
		QString val;
		
		if ( action == aOthersValuesAddValue )
		{
			val	= QInputDialog::getText( window(), title, tr( "Enter the value :" ), QLineEdit::Normal, QString(), &ok );
			if ( !ok )
			{
				val.clear();
			}
		}
		else if ( action == aOthersValuesAddFile )
		{
			val = QFileDialog::getOpenFileName( window(), tr( "Choose a file" ), mProject->path() );
			if ( !val.isEmpty() )
			{
				val = mProject->relativeFilePath( val );
			}
		}
		else if ( action == aOthersValuesAddPath )
		{
			val = QFileDialog::getExistingDirectory( window(), tr( "Choose a path" ), mProject->path() );
			if ( !val.isEmpty() )
			{
				val = mProject->relativeFilePath( val );
			}
		}
		
		if ( !val.isEmpty() )
		{
			// quote value if needed
			if ( val.contains( " " ) && !val.startsWith( '"' ) && !val.endsWith( '"' ) )
			{
				val.prepend( '"' ).append( '"' );
			}
			
			// check if value exists
			for ( int i = 0; i < lwOthersValues->count(); i++ )
			{
				QListWidgetItem* valueItem = lwOthersValues->item( i );
				
				if ( valueItem->text() == val )
				{
					lwOthersValues->setCurrentItem( valueItem );
					return;
				}
			}
			
			// create value item
			QListWidgetItem* valueItem = new QListWidgetItem( val, lwOthersValues );
			lwOthersValues->setCurrentItem( valueItem );
		}
	}*/
}

void VariablesEditor::on_tbOthersValuesEdit_clicked()
{
	on_tbOthersValuesEdit_triggered( aOthersValuesEditValue );
}

void VariablesEditor::on_tbOthersValuesEdit_triggered( QAction* action )
{
	/*QListWidgetItem* valueItem = lwOthersValues->currentItem();
	
	if ( valueItem )
	{
		const QString title = tr( "Edit a value..." );
		bool ok = true;
		QString oldValue = valueItem->text();
		QString val;
		
		if ( action == aOthersValuesEditValue )
		{
			val	= QInputDialog::getText( window(), title, tr( "Edit the value :" ), QLineEdit::Normal, oldValue, &ok );
			if ( !ok )
			{
				val.clear();
			}
		}
		else if ( action == aOthersValuesEditFile )
		{
			val = QFileDialog::getOpenFileName( window(), tr( "Choose a file" ), oldValue );
			if ( !val.isEmpty() )
			{
				val = mProject->relativeFilePath( val );
			}
		}
		else if ( action == aOthersValuesEditPath )
		{
			val = QFileDialog::getExistingDirectory( window(), tr( "Choose a path" ), oldValue );
			if ( !val.isEmpty() )
			{
				val = mProject->relativeFilePath( val );
			}
		}
		
		if ( !val.isEmpty() )
		{
			// quote value if needed
			if ( val.contains( " " ) && !val.startsWith( '"' ) && !val.endsWith( '"' ) )
			{
				val.prepend( '"' ).append( '"' );
			}
			
			// check if value exists
			for ( int i = 0; i < lwOthersValues->count(); i++ )
			{
				QListWidgetItem* item = lwOthersValues->item( i );
				
				if ( item->text() == val )
				{
					lwOthersValues->setCurrentItem( item );
					return;
				}
			}
			
			// update item
			valueItem->setText( val );
		}
	}*/
}

void VariablesEditor::on_tbOthersValuesClear_clicked()
{
	/*QListWidgetItem* variableItem = lwOthersVariables->currentItem();
	
	if ( variableItem )
	{
		// request user confirm
		if ( QMessageBox::question( QApplication::activeWindow(), tr( "Clear values..." ), tr( "A you sure you want to clear these values ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
		{
			lwOthersValues->clear();
		}
	}*/
}

#include "VariablesEditor.moc"
