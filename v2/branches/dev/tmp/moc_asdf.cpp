/****************************************************************************
** Meta object code from reading C++ file 'asdf.h'
**
** Created: Thu Nov 8 22:57:17 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "asdf.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'asdf.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_pStringListEditor[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_pStringListEditor[] = {
    "pStringListEditor\0\0onActionTriggered(QAction*)\0"
};

const QMetaObject pStringListEditor::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_pStringListEditor,
      qt_meta_data_pStringListEditor, 0 }
};

const QMetaObject *pStringListEditor::metaObject() const
{
    return &staticMetaObject;
}

void *pStringListEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_pStringListEditor))
	return static_cast<void*>(const_cast< pStringListEditor*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int pStringListEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onActionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
