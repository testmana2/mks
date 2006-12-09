/****************************************************************************
** Meta object code from reading C++ file 'qscilexerperl.h'
**
** Created: Thu Oct 26 22:14:42 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qscilexerperl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscilexerperl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciLexerPerl[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x0a,
      42,   15,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QsciLexerPerl[] = {
    "QsciLexerPerl\0\0fold\0setFoldComments(bool)\0setFoldCompact(bool)\0"
};

const QMetaObject QsciLexerPerl::staticMetaObject = {
    { &QsciLexer::staticMetaObject, qt_meta_stringdata_QsciLexerPerl,
      qt_meta_data_QsciLexerPerl, 0 }
};

const QMetaObject *QsciLexerPerl::metaObject() const
{
    return &staticMetaObject;
}

void *QsciLexerPerl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciLexerPerl))
	return static_cast<void*>(const_cast<QsciLexerPerl*>(this));
    return QsciLexer::qt_metacast(_clname);
}

int QsciLexerPerl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QsciLexer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setFoldComments((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: setFoldCompact((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
