/****************************************************************************
** Meta object code from reading C++ file 'QMakeProjectProxyModel.h'
**
** Created: Tue Apr 10 19:59:10 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/model/QMakeProjectProxyModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QMakeProjectProxyModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QMakeProjectProxyModel[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QMakeProjectProxyModel[] = {
    "QMakeProjectProxyModel\0"
};

const QMetaObject QMakeProjectProxyModel::staticMetaObject = {
    { &QSortFilterProxyModel::staticMetaObject, qt_meta_stringdata_QMakeProjectProxyModel,
      qt_meta_data_QMakeProjectProxyModel, 0 }
};

const QMetaObject *QMakeProjectProxyModel::metaObject() const
{
    return &staticMetaObject;
}

void *QMakeProjectProxyModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMakeProjectProxyModel))
	return static_cast<void*>(const_cast< QMakeProjectProxyModel*>(this));
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int QMakeProjectProxyModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
