/****************************************************************************
** Meta object code from reading C++ file 'qscimacro.h'
**
** Created: Thu Oct 26 22:14:49 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qscimacro.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscimacro.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciMacro[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      18,   10,   10,   10, 0x0a,
      35,   10,   10,   10, 0x0a,
      68,   50,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QsciMacro[] = {
    "QsciMacro\0\0play()\0startRecording()\0endRecording()\0"
    "msg,wParam,lParam\0record(uint,ulong,long)\0"
};

const QMetaObject QsciMacro::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QsciMacro,
      qt_meta_data_QsciMacro, 0 }
};

const QMetaObject *QsciMacro::metaObject() const
{
    return &staticMetaObject;
}

void *QsciMacro::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciMacro))
	return static_cast<void*>(const_cast<QsciMacro*>(this));
    return QObject::qt_metacast(_clname);
}

int QsciMacro::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: play(); break;
        case 1: startRecording(); break;
        case 2: endRecording(); break;
        case 3: record((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2])),(*reinterpret_cast< long(*)>(_a[3]))); break;
        }
        _id -= 4;
    }
    return _id;
}
