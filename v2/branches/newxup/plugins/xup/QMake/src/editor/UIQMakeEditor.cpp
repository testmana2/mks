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
		<< new QMakeVariablesEditor( mPositiveValues, mNegativeValues )
		<< new CommandsEditor
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
	// clear values, it will be update by page finalize
	qWarning() << mPositiveValues;
	qWarning() << mNegativeValues;
	
	mPositiveValues.clear();
	mNegativeValues.clear();
	
	UIXUPEditor::finalize();
	
	// update proejct items
	qWarning() << mPositiveValues;
	qWarning() << mNegativeValues;
}

void UIQMakeEditor::initializeVariables( XUPProjectItem* project )
{
	/*
		Loading datas from variable of root scope having operator =, += or *= only
		and not being FILE or PATH based.
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
			
			if ( blackList.contains( variableName ) ) {
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
	
	/*if ( mValues.value( "CONFIG" ).isEmpty() ) {
		mValues[ "CONFIG" ]
			<< "lex"
			<< "yacc"
			<< "uic"
			<< "resources"
			<< "qt"
			<< "warn_on"
			<< "incremental"
			<< "link_prl"
			<< "def_files_disabled"
			<< "exceptions"
			<< "no_mocdepend"
			<< "stl"
			<< "qt_no_framework"
			;
	}
	
	if ( mValues.value( "CONFIG" ).contains( "qt" )
		&& mValues.value( "QT" ).isEmpty() ) {
		mValues[ "QT" ]
			<< "core"
			<< "gui"
			;
	}*/
	
	qWarning() << mPositiveValues;
	qWarning() << mNegativeValues;
}
