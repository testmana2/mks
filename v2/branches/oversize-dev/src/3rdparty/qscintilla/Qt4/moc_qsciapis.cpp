/****************************************************************************
** Meta object code from reading C++ file 'qsciapis.h'
**
** Created: Thu Oct 26 22:14:29 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qsciapis.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsciapis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciAPIs[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      26,    9,    9,    9, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_QsciAPIs[] = {
    "QsciAPIs\0\0workerStarted()\0workerFinished()\0"
};

const QMetaObject QsciAPIs::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QsciAPIs,
      qt_meta_data_QsciAPIs, 0 }
};

const QMetaObject *QsciAPIs::metaObject() const
{
    return &staticMetaObject;
}

void *QsciAPIs::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciAPIs))
	return static_cast<void*>(const_cast<QsciAPIs*>(this));
    return QObject::qt_metacast(_clname);
}

int QsciAPIs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: workerStarted(); break;
        case 1: workerFinished(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QsciAPIs::workerStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QsciAPIs::workerFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
