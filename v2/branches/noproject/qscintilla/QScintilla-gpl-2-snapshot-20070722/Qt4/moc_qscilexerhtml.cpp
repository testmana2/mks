/****************************************************************************
** Meta object code from reading C++ file 'qscilexerhtml.h'
**
** Created: Sat Sep 15 16:32:02 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Qsci/qscilexerhtml.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscilexerhtml.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciLexerHTML[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      20,   15,   14,   14, 0x0a,
      41,   15,   14,   14, 0x0a,
      72,   67,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QsciLexerHTML[] = {
    "QsciLexerHTML\0\0fold\0setFoldCompact(bool)\0"
    "setFoldPreprocessor(bool)\0sens\0"
    "setCaseSensitiveTags(bool)\0"
};

const QMetaObject QsciLexerHTML::staticMetaObject = {
    { &QsciLexer::staticMetaObject, qt_meta_stringdata_QsciLexerHTML,
      qt_meta_data_QsciLexerHTML, 0 }
};

const QMetaObject *QsciLexerHTML::metaObject() const
{
    return &staticMetaObject;
}

void *QsciLexerHTML::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciLexerHTML))
	return static_cast<void*>(const_cast< QsciLexerHTML*>(this));
    return QsciLexer::qt_metacast(_clname);
}

int QsciLexerHTML::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QsciLexer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setFoldCompact((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: setFoldPreprocessor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: setCaseSensitiveTags((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
