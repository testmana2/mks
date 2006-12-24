#ifndef QMAKEPROJECTITEM_H
#define QMAKEPROJECTITEM_H
//
#include <QStandardItem>
//
class QMakeProjectItem : public QStandardItem
{
public:
	enum NodeRole
	{
		TypeRole = Qt::UserRole, // the item type
		AbsoluteFilePathRole, // absolute path in case of type Project, Folder ou File
		ProjectFilePathRole, // original filepath
		NegateRole, // bool
		//NestedRole, // nested ie: else, win2 etc
		ParametersRole, // function parameters as QStringList
		CommentRole, // a comment line after value
		OperatorRole // an operator like -=, = etc
	};
	//
	enum NodeType
	{
		None = QStandardItem::UserType +1, // 1001
		Project, // PROJECT
		Variable, // HEADERS, SOURCES ...
		Value, // a value of a variable
		Scope, //
		/*
		SingleLineScope, // win32:HEADERS
		NestedScope, // win32:debug
		MultiLineScope, // win32 {}
		*/
		Function, // message ()
		EmptyLine, // empty line
		Comment, // #
		Folder, // a group of file value
		File // a file value
	};
	//
	QMakeProjectItem( QMakeProjectItem::NodeType );
	~QMakeProjectItem();
	//
	virtual int type() const;
	//
private:
	void initialize();
	//
	QMakeProjectItem::NodeType mType;
	//
};
//
#endif // QMAKEPROJECTITEM_H
