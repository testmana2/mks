/****************************************************************************
** Meta object code from reading C++ file 'QMakeProjectItem.h'
**
** Created: Tue Apr 10 19:59:08 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/item/QMakeProjectItem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMakeProjectItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QMakeProjectItem[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       2,   10, // enums/sets

 // enums: name, flags, count, data
      17, 0x0,   10,   18,
      26, 0x0,   12,   38,

 // enum data: key, value
      35, uint(QMakeProjectItem::FirstRole),
      45, uint(QMakeProjectItem::TypeRole),
      54, uint(QMakeProjectItem::OperatorRole),
      67, uint(QMakeProjectItem::ValueRole),
      77, uint(QMakeProjectItem::CommentRole),
      89, uint(QMakeProjectItem::ProjectRole),
     101, uint(QMakeProjectItem::AbsoluteFilePathRole),
     122, uint(QMakeProjectItem::FileRole),
     131, uint(QMakeProjectItem::FolderRole),
     142, uint(QMakeProjectItem::LastRole),
     151, uint(QMakeProjectItem::FirstType),
     161, uint(QMakeProjectItem::EmptyType),
     171, uint(QMakeProjectItem::CommentType),
     183, uint(QMakeProjectItem::NestedScopeType),
     199, uint(QMakeProjectItem::ScopeType),
     209, uint(QMakeProjectItem::ScopeEndType),
     222, uint(QMakeProjectItem::VariableType),
     235, uint(QMakeProjectItem::ValueType),
     245, uint(QMakeProjectItem::FunctionType),
     258, uint(QMakeProjectItem::IncludeType),
     270, uint(QMakeProjectItem::ProjectType),
     282, uint(QMakeProjectItem::LastType),

       0        // eod
};

static const char qt_meta_stringdata_QMakeProjectItem[] = {
    "QMakeProjectItem\0NodeRole\0NodeType\0"
    "FirstRole\0TypeRole\0OperatorRole\0"
    "ValueRole\0CommentRole\0ProjectRole\0"
    "AbsoluteFilePathRole\0FileRole\0FolderRole\0"
    "LastRole\0FirstType\0EmptyType\0CommentType\0"
    "NestedScopeType\0ScopeType\0ScopeEndType\0"
    "VariableType\0ValueType\0FunctionType\0"
    "IncludeType\0ProjectType\0LastType\0"
};

const QMetaObject QMakeProjectItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QMakeProjectItem,
      qt_meta_data_QMakeProjectItem, 0 }
};

const QMetaObject *QMakeProjectItem::metaObject() const
{
    return &staticMetaObject;
}

void *QMakeProjectItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMakeProjectItem))
	return static_cast<void*>(const_cast< QMakeProjectItem*>(this));
    return QObject::qt_metacast(_clname);
}

int QMakeProjectItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
