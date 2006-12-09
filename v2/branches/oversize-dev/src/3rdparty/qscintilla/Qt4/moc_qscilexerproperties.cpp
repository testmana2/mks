/****************************************************************************
** Meta object code from reading C++ file 'qscilexerproperties.h'
**
** Created: Thu Oct 26 22:14:44 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qscilexerproperties.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscilexerproperties.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciLexerProperties[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      26,   21,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QsciLexerProperties[] = {
    "QsciLexerProperties\0\0fold\0setFoldCompact(bool)\0"
};

const QMetaObject QsciLexerProperties::staticMetaObject = {
    { &QsciLexer::staticMetaObject, qt_meta_stringdata_QsciLexerProperties,
      qt_meta_data_QsciLexerProperties, 0 }
};

const QMetaObject *QsciLexerProperties::metaObject() const
{
    return &staticMetaObject;
}

void *QsciLexerProperties::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciLexerProperties))
	return static_cast<void*>(const_cast<QsciLexerProperties*>(this));
    return QsciLexer::qt_metacast(_clname);
}

int QsciLexerProperties::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QsciLexer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setFoldCompact((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
