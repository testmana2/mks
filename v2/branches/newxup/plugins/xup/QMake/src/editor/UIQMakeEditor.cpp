#include "UIQMakeEditor.h"
#include "QMakeMainEditor.h"
#include "QMakeConfigurationEditor.h"
#include "QMakeFilesEditor.h"
#include "QMakeVariablesEditor.h"
#include "CommandsEditor.h"

#include <XUPProjectItem.h>

#include <QSet>
#include <QDebug>

UIQMakeEditor::UIQMakeEditor( QWidget* parent )
	: UIXUPEditor( parent )
{
}

UIQMakeEditor::~UIQMakeEditor()
{
}

void UIQMakeEditor::setupProject( XUPProjectItem* project )
{
	initializeVariables( project );
	
	const QList<XUPPageEditor*> pages = QList<XUPPageEditor*>()
		<< new QMakeMainEditor( mPositiveValues, mNegativeValues )
		<< new QMakeConfigurationEditor( mPositiveValues, mNegativeValues )
		<< new QMakeFilesEditor( mPositiveValues, mNegativeValues )
		/*<< new QMakeVariablesEditor( mPositiveValues, mNegativeValues )
		<< new CommandsEditor*/
		;
	
	addPages( pages );
	setup( project );
}

bool UIQMakeEditor::showProjectFilesPage()
{
	/*for ( int i = 0; i < mPages.count(); i++ ) {
		const XUPPageEditor* page = mPages.at( i );
		
		if ( page->inherits( "FilesEditor" ) ) {
			setCurrentPage( i );
			return true;
		}
	}*/
	
	return false;
}

void UIQMakeEditor::finalize()
{
	qWarning() << "*** BEFORE";
	qWarning() << mPositiveValues;
	qWarning() << mNegativeValues;
	
	// clear values, it will be update by pages finalize
	foreach ( const QString& variableName, mPositiveValues.keys() ) {
		mPositiveValues[ variableName ].clear();
	}
	
	foreach ( const QString& variableName, mNegativeValues.keys() ) {
		mNegativeValues[ variableName ].clear();
	}
	
	UIXUPEditor::finalize();
	
	// write positive values
	foreach ( const QString& variableName, mPositiveValues.keys() ) {
		updateVariable( mProject, variableName, true, mPositiveValues[ variableName ] );
	}
	
	// write negative values
	foreach ( const QString& variableName, mNegativeValues.keys() ) {
		updateVariable( mProject, variableName, false, mNegativeValues[ variableName ] );
	}
	
	// update proejct items
	qWarning() << "*** AFTER";
	qWarning() << mPositiveValues;
	qWarning() << mNegativeValues;
}

void UIQMakeEditor::initializeVariables( XUPProjectItem* project )
{
	/*
		Loading datas from variable of root scope having operator =, += or *= only
		and not being FILE or PATH based and not being custom xup variables
	*/
	
	const DocumentFilterMap& filters = project->documentFilters();
	const QSet<QString> positiveOperators = QSet<QString>() << "=" << "+=" << "*=";
	const QSet<QString> negativeOperators = QSet<QString>() << "-=";
	const QSet<QString> blackList = (
			QStringList()
				<< filters.fileVariables()
				<< filters.pathVariables()
		).toSet();
	
	foreach ( XUPItem* child, project->childrenList() ) {
		if ( child->type() == XUPItem::Variable ) {
			const QString variableName = child->attribute( "name" );
			const QString op = child->attribute( "operator", "=" );
			const bool isPositive = positiveOperators.contains( op );
			const bool isNegative = negativeOperators.contains( op );
			QStringList values;
			
			if ( blackList.contains( variableName ) || variableName.startsWith( XUPProjectItemHelper::SettingsScopeName ) ) {
				continue;
			}
			
			foreach ( XUPItem* value, child->childrenList() ) {
				if ( value->type() != XUPItem::Value ) {
					continue;
				}
				
				values << value->content();
			}
			
			if ( !values.isEmpty() ) {
				// some magic to remove duplicate entries
				const QSet<QString> entries = filters.splitValue( values.join( " " ) ).toSet();
				
				if ( isPositive ) {
					mPositiveValues[ variableName ] = entries.toList();
				}
				else if ( isNegative ) {
					mNegativeValues[ variableName ] = entries.toList();
				}
			}
		}
	}
	
	qWarning() << "*** INITIALIZE";
	qWarning() << mPositiveValues;
	qWarning() << mNegativeValues;
}

XUPItem* UIQMakeEditor::uniqueVariable( XUPItem* scope, const QString& variableName, bool positive, bool create )
{
	const QStringList operators = positive ? QStringList() << "=" << "*=" << "+=" : QStringList( "-=" );
	const XUPItemList variables = scope->project()->getVariables( scope, variableName, false );
	XUPItem* variableItem = 0;
	
	// remove duplicate variables
	foreach ( XUPItem* variable, variables ) {
		const QString op = variable->attribute( "operator", "=" );
		
		if ( !variableItem && operators.contains( op ) ) {
			variableItem = variable;
		}
		else if ( operators.contains( op ) ) {
			variable->parent()->removeChild( variable );
		}
	}
	
	// create it if needed
	if ( !variableItem && create ) {
		variableItem = scope->addChild( XUPItem::Variable );
		variableItem->setAttribute( "name", variableName );
	}
	
	// set / update operator
	if ( variableItem ) {
		QString op = variableItem->attribute( "operator", "=" );
		
		if ( positive ) {
			op = op == "*=" ? op : ( variableName == "CONFIG" ? "*=" : "=" );
		}
		else {
			op = "-=";
		}
		
		variableItem->setAttribute( "operator", op );
	}
	
	// return item
	return variableItem;
}

void UIQMakeEditor::updateVariable( XUPItem* scope, const QString& variableName, bool positive, const QStringList& values )
{
	XUPItem* variableItem = uniqueVariable( scope, variableName, positive, !values.isEmpty() );
	
	if ( !variableItem ) {
		return;
	}
	
	// remove all child values
	foreach ( XUPItem* child, variableItem->childrenList() ) {
		if ( child->type() == XUPItem::Value ) {
			variableItem->removeChild( child );
		}
	}
	
	// add new one
	foreach ( const QString& value, values ) {
		XUPItem* valueItem = variableItem->addChild( XUPItem::Value );
		valueItem->setContent( value );
	}
	
	// remove variable if it's empty
	if( !variableItem->hasChildren() ) {
		variableItem->parent()->removeChild( variableItem );
	}
}
