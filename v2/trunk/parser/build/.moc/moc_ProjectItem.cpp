/****************************************************************************
** Meta object code from reading C++ file 'ProjectItem.h'
**
** Created: Sun Apr 1 20:49:38 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/3rdparty/parser/ProjectItem.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProjectItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ProjectItem[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       2,   10, // enums/sets

 // enums: name, flags, count, data
      12, 0x0,   10,   18,
      21, 0x0,   10,   38,

 // enum data: key, value
      30, uint(ProjectItem::TypeRole),
      39, uint(ProjectItem::ValueRole),
      49, uint(ProjectItem::IndentRole),
      60, uint(ProjectItem::CommentRole),
      72, uint(ProjectItem::OperatorRole),
      85, uint(ProjectItem::AbsoluteFilePathRole),
     106, uint(ProjectItem::SortRole),
     115, uint(ProjectItem::SortRole2),
     125, uint(ProjectItem::FilterRole),
     136, uint(ProjectItem::ProjectRole),
     148, uint(ProjectItem::None),
     153, uint(ProjectItem::Line),
     158, uint(ProjectItem::Scope),
     164, uint(ProjectItem::ScopeEnd),
     173, uint(ProjectItem::Variable),
     182, uint(ProjectItem::Value),
     188, uint(ProjectItem::File),
     193, uint(ProjectItem::Folder),
     200, uint(ProjectItem::Function),
     209, uint(ProjectItem::Project),

       0        // eod
};

static const char qt_meta_stringdata_ProjectItem[] = {
    "ProjectItem\0NodeRole\0NodeType\0TypeRole\0"
    "ValueRole\0IndentRole\0CommentRole\0"
    "OperatorRole\0AbsoluteFilePathRole\0"
    "SortRole\0SortRole2\0FilterRole\0ProjectRole\0"
    "None\0Line\0Scope\0ScopeEnd\0Variable\0"
    "Value\0File\0Folder\0Function\0Project\0"
};

const QMetaObject ProjectItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ProjectItem,
      qt_meta_data_ProjectItem, 0 }
};

const QMetaObject *ProjectItem::metaObject() const
{
    return &staticMetaObject;
}

void *ProjectItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProjectItem))
	return static_cast<void*>(const_cast< ProjectItem*>(this));
    if (!strcmp(_clname, "QStandardItem"))
	return static_cast< QStandardItem*>(const_cast< ProjectItem*>(this));
    return QObject::qt_metacast(_clname);
}

int ProjectItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
