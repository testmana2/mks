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
		mRootItem = 0;
		mDocumentFilterMap = 0;
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
			const StringItem* parent = mMapping.value( item );
			bool enabled = parent && parent != &mRoot ? parent->enabled : item->enabled;
			
			if ( enabled ) {
				enabled = item->enabled;
			}
			
			switch ( role ) {
				case Qt::DisplayRole:
				case Qt::EditRole:
					return item->string;
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
	
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const
	{
		if ( column != 0 ) {
			return QModelIndex();
		}
		
		StringItem* item = this->item( parent );
		return item && row < item->children.count() ? createIndex( row, column, item ) : QModelIndex();
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
		StringItem* item = parentItem && !parentItem->children.isEmpty() ? &( parentItem->children[ index.row() ] ) : 0;
		return index == QModelIndex() ? &mRoot : item;
	}
	
	void clear()
	{
		const int count = mRoot.children.count();
		
		if ( count > 0 ) {
			beginRemoveRows( QModelIndex(), 0, count -1 );
			mRoot = StringItem();
			mMapping.clear();
			mRootItem = 0;
			mDocumentFilterMap = 0;
			mManagedVariables.clear();
			endRemoveRows();
		}
	}
	
	void setRootItem( const StringItem& item )
	{
		setRootItemInternal( item, true );
	}
	
	void setRootItem( XUPItem* item )
	{
		mRootItem = item;
		revert();
	}
	
	QModelIndex addChild( const QModelIndex& index, const StringItem& child )
	{
		const QModelIndex childIndex = this->childIndex( index, child.string );
		
		if ( childIndex.isValid() ) {
			return childIndex;
		}
		
		StringItem* item = this->item( index );
		
		if ( item ) {
			const int count = item->children.count();
			beginInsertRows( index, count, count );
			item->children << child;
			mMapping[ &( item->children.last() ) ] = item;
			endInsertRows();
			return createIndex( item->children.count() -1, 0, item );
		}
		
		return QModelIndex();
	}
	
	QModelIndex childIndex( const QModelIndex& index, const QString& string ) const
	{
		StringItem* item = this->item( index );
		
		if ( item ) {
			for ( int i = 0; i < item->children.count(); i++ ) {
				StringItem& it = item->children[ i ];
				
				if ( it.string == string ) {
					return createIndex( i, 0, item );
				}
			}
		}
		
		return QModelIndex();
	}
	
	QStringList fileVariables() const { return mDocumentFilterMap ? mDocumentFilterMap->fileVariables() : QStringList(); }
	QStringList pathVariables() const { return mDocumentFilterMap ? mDocumentFilterMap->pathVariables() : QStringList(); }
	QStringList knownVariables() const { return mDocumentFilterMap ? mDocumentFilterMap->knownVariables() : QStringList(); }
	QStringList managedVariables() const { return mManagedVariables; }

public slots:
	virtual void revert()
	{
		XUPItem* item = mRootItem;
		
		if ( !mRootItem ) {
			return;
		}
		
		clear();
		
		const XUPProjectItem* project = item->project();
		mRootItem = item;
		mDocumentFilterMap = &project->documentFilters();
		mManagedVariables = mDocumentFilterMap->fileVariables();
		StringItem root;
		
		// only retreives values of variables that are direct children of item
		foreach ( XUPItem* variable, mRootItem->childrenList() ) {
			if ( variable->type() == XUPItem::Variable ) {
				StringItem si( variable->attribute( "name" ), true, variable );
				
				if ( si.string.startsWith( XUPProjectItemHelper::SettingsScopeName ) || mManagedVariables.contains( si.string ) ) {
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
		
		setRootItemInternal( root, false );
	}
	
	virtual bool submit()
	{
		if ( !mRootItem ) {
			return false;
		}
		
		foreach ( const StringItem& variable, mRoot.children ) {
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
			
			foreach ( const StringItem& value, variable.children ) {
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

protected:
	XUPItem* mRootItem;
	const DocumentFilterMap* mDocumentFilterMap;
	mutable StringItem mRoot;
	QHash<StringItem*, StringItem*> mMapping;
	QStringList mManagedVariables;
	
	void buildParentMapping( StringItem& item )
	{
		if ( item == mRoot ) {
			mMapping[ &mRoot ] = 0;
		}
		
		for ( int i = 0; i < item.children.count(); i++ ) {
			StringItem& child = item.children[ i ];
			mMapping[ &child ] = &item;
			buildParentMapping( child );
		}
	}
	
	void setRootItemInternal( const StringItem& item, bool clearModel )
	{
		if ( clearModel ) {
			clear();
		}
		
		if ( item == StringItem() ) {
			return;
		}
		
		const int count = item.children.count();
		
		beginInsertRows( QModelIndex(), 0, count -1 );
		mRoot = item;
		mRoot.enabled = true;
		buildParentMapping( mRoot );
		endInsertRows();
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
	
	// tbValuesAdd actions
	QMenu* addMenu = new QMenu( tbValuesAdd );
	aValuesAddValue = addMenu->addAction( tr( "As Value..." ) );
	aValuesAddFile = addMenu->addAction( tr( "As File..." ) );
	aValuesAddPath = addMenu->addAction( tr( "As Path..." ) );
	tbValuesAdd->setMenu( addMenu );
	
	// tbValuesEdit actions
	QMenu* editMenu = new QMenu( tbValuesEdit );
	aValuesEditValue = editMenu->addAction( tr( "As Value..." ) );
	aValuesEditFile = editMenu->addAction( tr( "As File..." ) );
	aValuesEditPath = editMenu->addAction( tr( "As Path..." ) );
	tbValuesEdit->setMenu( editMenu );
	
	connect( lvVariables->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvVariables_selectionModel_selectionChanged() ) );
	connect( lvValues->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lvValues_selectionModel_selectionChanged() ) );
}

VariablesEditor::~VariablesEditor()
{
}

QStringList VariablesEditor::fileVariables() const
{
	return mModel->fileVariables();
}

QStringList VariablesEditor::pathVariables() const
{
	return mModel->pathVariables();
}

QStringList VariablesEditor::managedVariables() const
{
	return mModel->managedVariables();
}

void VariablesEditor::init( XUPProjectItem* project )
{
	mProject = project;
	mModel->setRootItem( project );
	const QModelIndex index = mModel->index( 0, 0 );
	lvVariables->setCurrentIndex( index );
	lvVariables->scrollTo( index );
	lvVariables_selectionModel_selectionChanged();
}

void VariablesEditor::finalize()
{
	mModel->submit();
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

QString VariablesEditor::quotedString( const QString& string ) const
{
	if ( cbQuoteEnabled->isChecked() ) {
		const QString quote = cbQuote->currentText();
		
		if ( string.contains( " " ) && !string.startsWith( quote ) && !string.endsWith( quote ) ) {
			return QString( string ).prepend( quote ).append( quote );
		}
	}
	
	return string;
}

void VariablesEditor::lvVariables_selectionModel_selectionChanged()
{
	const QModelIndex index = lvVariables->selectionModel()->selectedIndexes().value( 0 );
	tbVariablesEdit->setEnabled( index.isValid() );
	gbValues->setEnabled( index.isValid() );
	lvValues->setRootIndex( index );
	lvValues_selectionModel_selectionChanged();
}

void VariablesEditor::on_tbVariablesAdd_clicked()
{
	const QStringList variables = mModel->knownVariables();
	bool ok;
	
	const QString variable = QInputDialog::getItem( QApplication::activeWindow(), tr( "Add variable..." ), tr( "Select a variable name or enter a new one" ), variables, 0, true, &ok );
	
	if ( !variable.isEmpty() && ok ) {
		const QModelIndex index = mModel->managedVariables().contains( variable ) ? QModelIndex() : mModel->addChild( QModelIndex(), StringItem( variable, true ) );
		
		if ( index.isValid() ) {
			lvVariables->setCurrentIndex( index );
			lvVariables->scrollTo( index );
		}
		else {
			QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This variable is filtered out." ) );
		}
	}
}

void VariablesEditor::on_tbVariablesEdit_clicked()
{
	const QModelIndex index = lvVariables->selectionModel()->selectedIndexes().value( 0 );
	
	if ( !index.isValid() ) {
		return;
	}
	
	bool ok;
	const QString oldVariable = index.data( Qt::DisplayRole ).toString();
	const QString variable = QInputDialog::getText( QApplication::activeWindow(), tr( "Edit variable..." ), tr( "Enter a new name for this variable" ), QLineEdit::Normal, oldVariable, &ok );
	
	if ( !variable.isEmpty() && ok ) {
		const QModelIndex childIndex = mModel->childIndex( QModelIndex(), variable );
		
		if ( ( childIndex == index || !childIndex.isValid() ) && !mModel->managedVariables().contains( variable ) ) {
			mModel->setData( index, variable, Qt::DisplayRole );
		}
		else {
			QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This variable exists or is filtered out." ) );
		}
	}
}

void VariablesEditor::lvValues_selectionModel_selectionChanged()
{
	const QModelIndex index = lvValues->selectionModel()->selectedIndexes().value( 0 );
	tbValuesEdit->setEnabled( index.isValid() );
	tbValuesClear->setEnabled( index.isValid() );
}

void VariablesEditor::on_tbValuesAdd_clicked()
{
	on_tbValuesAdd_triggered( aValuesAddValue );
}

void VariablesEditor::on_tbValuesAdd_triggered( QAction* action )
{
	const QModelIndex variableIndex = lvVariables->selectionModel()->selectedIndexes().value( 0 );
	
	if ( !variableIndex.isValid() ) {
		return;
	}
	
	const QString title = tr( "Add a value..." );
	bool ok = true;
	QString value;
	
	if ( action == aValuesAddValue ) {
		value = QInputDialog::getText( QApplication::activeWindow(), title, tr( "Enter the value :" ), QLineEdit::Normal, QString(), &ok );
		
		if ( !ok ) {
			value.clear();
		}
	}
	else if ( action == aValuesAddFile ) {
		value = QFileDialog::getOpenFileName( QApplication::activeWindow(), tr( "Choose a file" ), mProject->path() );
		
		if ( !value.isEmpty() ) {
			value = mProject->relativeFilePath( value );
		}
	}
	else if ( action == aValuesAddPath ) {
		value = QFileDialog::getExistingDirectory( QApplication::activeWindow(), tr( "Choose a path" ), mProject->path() );
		
		if ( !value.isEmpty() ) {
			value = mProject->relativeFilePath( value );
		}
	}
	
	if ( value.isEmpty() ) {
		return;
	}
	
	const QModelIndex index = mModel->addChild( variableIndex, StringItem( quotedString( value ), true ) );
	lvValues->setCurrentIndex( index );
	lvValues->scrollTo( index );
}

void VariablesEditor::on_tbValuesEdit_clicked()
{
	on_tbValuesEdit_triggered( aValuesEditValue );
}

void VariablesEditor::on_tbValuesEdit_triggered( QAction* action )
{
	const QModelIndex variableIndex = lvVariables->selectionModel()->selectedIndexes().value( 0 );
	const QModelIndex valueIndex = lvValues->selectionModel()->selectedIndexes().value( 0 );
	
	if ( !variableIndex.isValid() || !valueIndex.isValid() ) {
		return;
	}
	
	const QString title = tr( "Edit a value..." );
	bool ok = true;
	QString oldValue = valueIndex.data( Qt::DisplayRole ).toString();
	QString value;
	
	if ( action == aValuesEditValue ) {
		value	= QInputDialog::getText( QApplication::activeWindow(), title, tr( "Edit the value :" ), QLineEdit::Normal, oldValue, &ok );
		
		if ( !ok ) {
			value.clear();
		}
	}
	else if ( action == aValuesEditFile ) {
		value = QFileDialog::getOpenFileName( QApplication::activeWindow(), tr( "Choose a file" ), oldValue );
		
		if ( !value.isEmpty() ) {
			value = mProject->relativeFilePath( value );
		}
	}
	else if ( action == aValuesEditPath ) {
		value = QFileDialog::getExistingDirectory( QApplication::activeWindow(), tr( "Choose a path" ), oldValue );
		
		if ( !value.isEmpty() ) {
			value = mProject->relativeFilePath( value );
		}
	}
	
	if ( value.isEmpty() ) {
		return;
	}
	
	const QModelIndex childIndex = mModel->childIndex( variableIndex, value );
	
	if ( childIndex == valueIndex || !childIndex.isValid() ) {
		mModel->setData( valueIndex, value, Qt::DisplayRole );
	}
	else {
		QMessageBox::information( QApplication::activeWindow(), tr( "Information..." ), tr( "This value already exists." ) );
	}
}

void VariablesEditor::on_tbValuesClear_clicked()
{
	const QModelIndex variableIndex = lvVariables->selectionModel()->selectedIndexes().value( 0 );
	
	if ( !variableIndex.isValid() ) {
		return;
	}
	
	if ( QMessageBox::question( QApplication::activeWindow(), tr( "Clear values..." ), tr( "Are you sure you want to clear these values ?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No ) {
		return;
	}
	
	for ( int i = 0; i < mModel->rowCount( variableIndex ); i++ ) {
		const QModelIndex index = variableIndex.child( i, 0 );
		mModel->setData( index, Qt::Unchecked, Qt::CheckStateRole );
	}
}

#include "VariablesEditor.moc"
