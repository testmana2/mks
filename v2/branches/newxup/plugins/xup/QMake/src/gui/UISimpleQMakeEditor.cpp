#include "UISimpleQMakeEditor.h"
#include "XUPProjectItem.h"
#include "../QMakeProjectItem.h"
#include "../QMake.h"

#include <MkSFileDialog.h>
#include <pMonkeyStudio.h>
#include <MonkeyCore.h>
#include <PluginsManager.h>
#include <XUPProjectItemHelper.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

UISimpleQMakeEditor::UISimpleQMakeEditor( XUPProjectItem* project, QWidget* parent )
	: QDialog( parent )
{
	// init dialog
	setupUi( this );
	
	// set dialog infos
	setWindowIcon( project->displayIcon() );
	setWindowTitle( windowTitle().append( " - " ).append( project->displayText() ) );
	
	// set size hint for page item ( left panel )
	for ( int i = 0; i < lwPages->count(); i++ )
	{
		QListWidgetItem* item = lwPages->item( i );
		item->setSizeHint( QSize( 154, 40 ) );
	}
	
	// values editor actions
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
	
	// connections
	connect( lwQtModules, SIGNAL( itemSelectionChanged() ), this, SLOT( modules_itemSelectionChanged() ) );
	connect( lwModules, SIGNAL( itemSelectionChanged() ), this, SLOT( modules_itemSelectionChanged() ) );
	
	// init proejct settings dialog
	init( project );
	
	// set correct page
	lwPages->setCurrentRow( 1 );
}

UISimpleQMakeEditor::~UISimpleQMakeEditor()
{
}

void UISimpleQMakeEditor::updateValuesEditorVariables()
{
	QListWidgetItem* curItem = lwOthersVariables->selectedItems().value( 0 );
	const QString curVariable = curItem ? curItem->text() : QString::null;
	curItem = 0;
	
	lwOthersVariables->clear();
	lwOthersValues->clear();
	
	foreach ( const QString& variable, mValues.keys() )
	{
		if ( !mManagedVariables.contains( variable ) )
		{
			lwOthersVariables->addItem( variable );
			
			if ( variable == curVariable )
			{
				curItem = lwOthersVariables->item( lwOthersVariables->count() -1 );
				curItem->setSelected( true );
			}
		}
	}
}

void UISimpleQMakeEditor::updateValuesEditorValues( const QString& variable )
{
	const QStringList values = mProject->documentFilters().splitValue( mValues[ variable ] );
	
	lwOthersValues->clear();
	lwOthersValues->addItems( values );
}

void UISimpleQMakeEditor::init( XUPProjectItem* project )
{
	mConfigGui.clear();
	QString value;
	QStringList config;
	QStringList values;
	mProject = dynamic_cast<QMakeProjectItem*>( project );
	mValues.clear();
	mManagedVariables.clear();
	mManagedVariables << "TEMPLATE" << "CONFIG" << "TARGET" << "DESTDIR" << "DLLDESTDIR" << "QT" << project->documentFilters().fileVariables();
	
	foreach ( QAbstractButton* ab, wCompilerSettings->findChildren<QAbstractButton*>() )
	{
		if ( !ab->statusTip().isEmpty() )
		{
			mConfigGui << ab->statusTip();
		}
	}
	
	mConfigGui << "app_bundle" << "lib_bundle" << "embed_manifest_exe" << "embed_manifest_dll"
		/*<< "designer"*/ << "plugin" << "shared" << "dll" << "static" << "staticlib";
	
	lwQtVersion->clear();
	lwQtModules->clear();
	lwModules->clear();
	
	// qt modules
	foreach ( const QtItem& mi, manager->modules() )
	{
		QListWidgetItem* it = new QListWidgetItem( mi.Text, lwQtModules );
		it->setData( Qt::UserRole, QVariant::fromValue( mi ) );
	}
	
	// configuration
	foreach ( const QtItem& ci, manager->configurations() )
	{
		if ( !mConfigGui.contains( ci.Value, Qt::CaseInsensitive ) )
		{
			if ( ci.Text.toLower() != "lib only" && ci.Text.toLower() != "x11 only" && ci.Text.toLower() != "mac os x only" )
			{
				QListWidgetItem* it = new QListWidgetItem( ci.Text, lwModules );
				it->setData( Qt::UserRole, QVariant::fromValue( ci ) );
				
				if ( ci.Value.isEmpty() && ci.Variable.isEmpty() )
				{
					QFont font( it->font() );
					font.setBold( true );
					it->setFont( font );
					it->setBackground( QBrush( QColor( Qt::darkBlue ) ) );
					it->setForeground( QBrush( QColor( Qt::white ) ) );
					it->setTextAlignment( Qt::AlignCenter );
					it->setFlags( it->flags() ^ Qt::ItemIsUserCheckable );
				}
				else
					it->setCheckState( Qt::Unchecked );
			}
		}
	}
	
	// update gui
	
	// modules
	values = project->documentFilters().splitValue( mValues[ "QT" ] );
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* item = lwQtModules->item( i );
		QtItem mi = item->data( Qt::UserRole ).value<QtItem>();
		
		if ( values.contains( mi.Value ) )
		{
			item->setCheckState( Qt::Checked );
		}
		else
		{
			item->setCheckState( Qt::Unchecked );
		}
	}
	
	for ( int i = 0; i < lwModules->count(); i++ )
	{
		QListWidgetItem* item = lwModules->item( i );
		QtItem ci = item->data( Qt::UserRole ).value<QtItem>();
		
		if ( config.contains( ci.Value ) )
		{
			item->setCheckState( Qt::Checked );
			config.removeAll( ci.Value );
		}
		else if ( !ci.Value.isEmpty() )
		{
			item->setCheckState( Qt::Unchecked );
		}
	}
	
	// compiler settings
	foreach ( QAbstractButton* ab, wCompilerSettings->findChildren<QAbstractButton*>() )
	{
		if ( !ab->statusTip().isEmpty() )
		{
			if ( ab == cbBuildAll )
			{
				ab->setChecked( config.contains( ab->statusTip() ) && config.contains( "debug_and_release" ) );
				config.removeAll( "debug_and_release" );
			}
			else
			{
				ab->setChecked( config.contains( ab->statusTip() ) );
			}
		}
		else if ( ab == cbBundle )
		{
			ab->setChecked( config.contains( "app_bundle" ) || config.contains( "lib_bundle" ) );
			config.removeAll( "app_bundle" );
			config.removeAll( "lib_bundle" );
		}
		else if ( ab == cbManifest )
		{
			ab->setChecked( config.contains( "embed_manifest_exe" ) || config.contains( "embed_manifest_dll" ) );
			config.removeAll( "embed_manifest_exe" );
			config.removeAll( "embed_manifest_dll" );
		}
		
		config.removeAll( ab->statusTip() );
	}
	
	// custom configuration
	leCustomConfig->setText( config.join( " " ) );

	updateValuesEditorVariables();
}

XUPItem* UISimpleQMakeEditor::getUniqueVariableItem( const QString& variableName, bool create )
{
	const QStringList mOperators = QStringList() << "=" << "+=" << "*=";
	XUPItemList variables = mProject->getVariables( mProject, variableName, false );
	XUPItem* variableItem = 0;
	
	// remove duplicate variables
	foreach ( XUPItem* variable, variables )
	{
		QString op = variable->attribute( "operator", "=" );
		
		if ( !variableItem && mOperators.contains( op ) )
		{
			variableItem = variable;
		}
		else if ( mOperators.contains( op ) )
		{
			variable->parent()->removeChild( variable );
		}
	}
	
	// create it if needed
	if ( !variableItem && create )
	{
		variableItem = mProject->addChild( XUPItem::Variable );
		variableItem->setAttribute( "name", variableName );
	}
	
	// return item
	return variableItem;
}

void UISimpleQMakeEditor::modules_itemSelectionChanged()
{
	QListWidget* lw = qobject_cast<QListWidget*>( sender() );
	QListWidgetItem* it = lw ? lw->selectedItems().value( 0 ) : 0;
	
	teModuleHelp->clear();
	
	if ( it )
	{
		teModuleHelp->setHtml( it->data( Qt::UserRole ).value<QtItem>().Help );
	}
}

void UISimpleQMakeEditor::on_lwOthersVariables_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
	// enable/disable actions
	gbOthersValues->setEnabled( current );
	tbOthersVariablesEdit->setEnabled( current );
	tbOthersVariablesRemove->setEnabled( current );
	
	// save previous variable datas
	if ( previous )
	{
		const QString variable = previous->text();
		QStringList values;
		
		for ( int i = 0; i < lwOthersValues->count(); i++ )
		{
			values << lwOthersValues->item( i )->text();
		}
		
		mValues[ variable ] = values.join( " " );;
	}
	
	// update values view
	const QString variable = current ? current->text() : QString::null;
	updateValuesEditorValues( variable );
}

void UISimpleQMakeEditor::on_tbOthersVariablesAdd_clicked()
{
	bool ok;
	const QStringList variables;
	const QString variable = QInputDialog::getText( window(), tr( "Add variable..." ), tr( "Enter variable name" ), QLineEdit::Normal, QString::null, &ok );
	
	if ( !variable.isEmpty() && ok )
	{
		if ( !mValues.keys().contains( variable ) && !mManagedVariables.contains( variable ) )
		{
			QListWidgetItem* item = new QListWidgetItem( variable, lwOthersVariables );
			lwOthersVariables->setCurrentItem( item );
			
			mValues[ variable ] = QString::null;
			mVariablesToRemove.removeAll( variable );
		}
		else
		{
			QMessageBox::information( window(), tr( "Information..." ), tr( "This variable already exists or is filtered out." ) );
		}
	}
}

void UISimpleQMakeEditor::on_tbOthersVariablesEdit_clicked()
{
	QListWidgetItem* item = lwOthersVariables->currentItem();
	
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
			QMessageBox::information( window(), tr( "Information..." ), tr( "This variable already exists or is filtered out." ) );
		}
	}
}

void UISimpleQMakeEditor::on_tbOthersVariablesRemove_clicked()
{
	QListWidgetItem* item = lwOthersVariables->currentItem();
	
	if ( !item )
	{
		return;
	}
	
	// confirm user request
	if ( QMessageBox::question( QApplication::activeWindow(), tr( "Remove a variable..." ), tr( "A you sure you want to remove this variable and all its content ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
	{
		QString variable = item->text();
		
		lwOthersValues->clear();
		delete item;
		
		mValues.remove( variable );
		if ( !mVariablesToRemove.contains( variable ) )
		{
			mVariablesToRemove << variable;
		}
	}
}

void UISimpleQMakeEditor::on_lwOthersValues_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
	// enable button according to item validity
	tbOthersValuesEdit->setEnabled( current );
	tbOthersValuesRemove->setEnabled( current );
	Q_UNUSED( previous );
}

void UISimpleQMakeEditor::on_tbOthersValuesAdd_clicked()
{
	on_tbOthersValuesAdd_triggered( aOthersValuesAddValue );
}

void UISimpleQMakeEditor::on_tbOthersValuesAdd_triggered( QAction* action )
{
	QListWidgetItem* variableItem = lwOthersVariables->currentItem();
	
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
	}
}

void UISimpleQMakeEditor::on_tbOthersValuesEdit_clicked()
{
	on_tbOthersValuesEdit_triggered( aOthersValuesEditValue );
}

void UISimpleQMakeEditor::on_tbOthersValuesEdit_triggered( QAction* action )
{
	QListWidgetItem* valueItem = lwOthersValues->currentItem();
	
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
	}
}

void UISimpleQMakeEditor::on_tbOthersValuesRemove_clicked()
{
	QListWidgetItem* valueItem = lwOthersValues->currentItem();
	
	if ( valueItem )
	{
		// confirm user request
		if ( QMessageBox::question( QApplication::activeWindow(), tr( "Remove a value..." ), tr( "A you sure you want to remove this value ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::No )
		{
			return;
		}
		
		// remove value
		delete valueItem;
	}
}

void UISimpleQMakeEditor::on_tbOthersValuesClear_clicked()
{
	QListWidgetItem* variableItem = lwOthersVariables->currentItem();
	
	if ( variableItem )
	{
		// request user confirm
		if ( QMessageBox::question( QApplication::activeWindow(), tr( "Clear values..." ), tr( "A you sure you want to clear these values ?" ), QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
		{
			lwOthersValues->clear();
		}
	}
}

void UISimpleQMakeEditor::accept()
{
	const DocumentFilterMap& filters = mProject->documentFilters();
	const QStringList fileVariables = filters.fileVariables();
	const QStringList pathVariables = filters.pathVariables();
	QString plugin;
	QString tmplate;
	QStringList config;
	QStringList qt;
	QStringList target;
	QString destdir;
	QString dlldestdir;
	
	// project
	if ( rbApplication->isChecked() )
	{
		tmplate = "app";
	}
	else if ( rbSubdirs->isChecked() )
	{
		tmplate = "subdirs";
	}
	else
	{
		tmplate = "lib";
		if ( rbQtDesignerPlugin->isChecked() )
		{
			config << "designer";
		}
		else if ( rbQtPlugin->isChecked() )
		{
			config << "plugin";
		}
		else if ( rbSharedLib->isChecked() )
		{
			config << "dll";
		}
		else if ( rbStaticLib->isChecked() )
		{
			config << "staticlib";
		}
	}
	
	if ( !rbSubdirs->isChecked() )
	{
		target << leProjectName->text();
		if ( rbApplication->isChecked() || rbStaticLib->isChecked() )
		{
			destdir = leProjectTarget->text();
		}
		else
		{
			dlldestdir = leProjectTarget->text();
		}
	}
	
	// modules
	for ( int i = 0; i < lwQtModules->count(); i++ )
	{
		QListWidgetItem* item = lwQtModules->item( i );
		if ( item->checkState() == Qt::Checked )
		{
			qt << item->data( Qt::UserRole ).value<QtItem>().Value;
		}
	}
	
	for ( int i = 0; i < lwModules->count(); i++ )
	{
		QListWidgetItem* item = lwModules->item( i );
		if ( item->checkState() == Qt::Checked )
		{
			config << item->data( Qt::UserRole ).value<QtItem>().Value;
		}
	}
	
	// compiler settings
	foreach ( QAbstractButton* ab, wCompilerSettings->findChildren<QAbstractButton*>() )
	{
		if ( !ab->isChecked() || !ab->isEnabled() )
		{
			continue;
		}
		
		if ( !ab->statusTip().isEmpty() )
		{
			config << ab->statusTip();
		}
		else
		{
			if ( ab == cbManifest )
			{
				if ( tmplate == "app" )
				{
					config << "embed_manifest_exe";
				}
				else if ( tmplate == "lib" )
				{
					config << "embed_manifest_dll";
				}
			}
			else if ( ab == cbBundle )
			{
				if ( tmplate == "app" )
				{
					config << "app_bundle";
				}
				else if ( tmplate == "lib" )
				{
					config << "lib_bundle";
				}
			}
		}
	}
	
	// save current variable if needed
	QListWidgetItem* curItem = lwOthersVariables->currentItem();
	on_lwOthersVariables_currentItemChanged( curItem, curItem );
	
	// custom configuration
	config << filters.splitValue( leCustomConfig->text() );
	
	mValues[ "TEMPLATE" ] = tmplate;
	mValues[ "CONFIG" ] = config.join( " " );
	mValues[ "QT" ] = qt.join( " " );
	mValues[ "TARGET" ] = target.join( " " );
	mValues[ "DESTDIR" ] = destdir;
	mValues[ "DLLDESTDIR" ] = dlldestdir;
	
	// tell about variables to remove
	foreach ( const QString& variable, mVariablesToRemove )
	{
		mValues[ variable ] = QString::null;
	}
	
	foreach ( const QString& variable, mValues.keys() )
	{
		bool isEmpty = mValues[ variable ].trimmed().isEmpty();
		XUPItem* variableItem = getUniqueVariableItem( variable, !isEmpty );
		if ( !variableItem )
		{
			continue;
		}
		
		if ( !isEmpty )
		{
			if ( fileVariables.contains( variable ) || pathVariables.contains( variable ) )
			{
				// get child type
				XUPItem::Type type = fileVariables.contains( variable ) ? XUPItem::File : XUPItem::Path;
				// get values
				QStringList values = filters.splitValue( mValues[ variable ] );
				
				// update variable
				variableItem->setAttribute( "operator", "+=" );
				variableItem->setAttribute( "multiline", "true" );
				
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
			else if ( variable == "CONFIG" )
			{
				// update variable
				variableItem->setAttribute( "operator", "+=" );
				variableItem->setAttribute( "multiline", "false" );
				
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
			else
			{
				// update variable
				variableItem->setAttribute( "operator", "=" );
				variableItem->setAttribute( "multiline", "false" );
				
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
	}
	
	// xup settings
	mQtVersion = QtVersion();
	QListWidgetItem* qtVersionItem = lwQtVersion->selectedItems().value( 0 );
	
	if ( qtVersionItem )
	{
		mQtVersion = qtVersionItem->data( Qt::UserRole ).value<QtVersion>();
	}

	XUPProjectItemHelper::setProjectSettingsValue( mProject, "QT_VERSION", mQtVersion.Version );
	
	// close dialog
	QDialog::accept();
}
