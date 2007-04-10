/****************************************************************************
** Meta object code from reading C++ file 'ProjectProxyModel.h'
**
** Created: Sun Apr 1 20:49:42 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/3rdparty/parser/ProjectProxyModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProjectProxyModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ProjectProxyModel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,   18,   18,   18, 0x08,
      54,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ProjectProxyModel[] = {
    "ProjectProxyModel\0\0sig_curPro(QString)\0"
    "setFilter(int)\0currentProjectName(QModelIndex)\0"
};

const QMetaObject ProjectProxyModel::staticMetaObject = {
    { &QSortFilterProxyModel::staticMetaObject, qt_meta_stringdata_ProjectProxyModel,
      qt_meta_data_ProjectProxyModel, 0 }
};

const QMetaObject *ProjectProxyModel::metaObject() const
{
    return &staticMetaObject;
}

void *ProjectProxyModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProjectProxyModel))
	return static_cast<void*>(const_cast< ProjectProxyModel*>(this));
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int ProjectProxyModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sig_curPro((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: setFilter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: currentProjectName((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ProjectProxyModel::sig_curPro(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
