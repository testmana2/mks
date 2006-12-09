/********************************************************************************************************
 * PROGRAM      : TextEditor
 * DATE - TIME  : lundi 01 mai 2006 - 12h02
 * AUTHOR       : Azevedo Filipe aka Nox PasNox ( pasnox at gmail dot com )
 * FILENAME     : Completion.h
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/
#ifndef COMPLETION_H
#define COMPLETION_H
//
#include <QFrame>
#include <QListWidget>
#include <QPointer>
#ifdef NO_PLUGIN
#define QDESIGNER_WIDGET_EXPORT
#else
#include <QDesignerExportWidget>
#endif
#include "InitCompletion.h"
//
class TextEditor;
class QAction;
class QVBoxLayout;
class QToolBar;
class QStatusBar;
//
class QDESIGNER_WIDGET_EXPORT Completion : public QFrame
{
	Q_OBJECT
	//
public:
	static Completion* self( QWidget* = 0, Qt::WFlags = Qt::Popup );
	Completion( QWidget* = 0, Qt::WFlags = Qt::Popup );
	QList<TextEditor*> editors();
	bool prepareCompletion();
	//
	enum ItemType { itVariables = QListWidgetItem::UserType +1, itMembers, itSlots, itSignals, itOthers };
	QAction* actionShowVariables;
	QAction* actionShowMembers;
	QAction* actionShowSignals;
	QAction* actionShowSlots;
	QAction* actionShowOthers;
	QVBoxLayout* vboxLayout;
	QToolBar* tbViews;
	QListWidget* lwItems;
	QStatusBar* sbInformations;
	
	InitCompletion* comp;
	//
public slots:
	void refreshView();
	void on_lwItems_itemDoubleClicked( QListWidgetItem* );
	void setEditors( const QList<TextEditor*>& );
	void invokeCompletion( TextEditor* );
	//
private:
	const QPoint mouse() const;
	void setMouse( const QPoint & );
	//
	QPoint mMouse;
	//
protected:
	void mouseMoveEvent ( QMouseEvent* );
	void mousePressEvent ( QMouseEvent* );
	//
	static QPointer<Completion> mSelf; // self widget
	TextEditor* mEditor; // the widget to use to insert text on popup clicked
	QList<TextEditor*> mEditors; // widget to scan to create popup list
	//
signals:
	void beforeCompletionShow();
	//
};
//
#endif // COMPLETION_H
