/****************************************************************************
** Meta object code from reading C++ file 'qscilexercpp.h'
**
** Created: Sat Sep 15 16:31:57 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Qsci/qscilexercpp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscilexercpp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciLexerCPP[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      19,   14,   13,   13, 0x0a,
      39,   14,   13,   13, 0x0a,
      61,   14,   13,   13, 0x0a,
      82,   14,   13,   13, 0x0a,
     114,  108,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QsciLexerCPP[] = {
    "QsciLexerCPP\0\0fold\0setFoldAtElse(bool)\0"
    "setFoldComments(bool)\0setFoldCompact(bool)\0"
    "setFoldPreprocessor(bool)\0style\0"
    "setStylePreprocessor(bool)\0"
};

const QMetaObject QsciLexerCPP::staticMetaObject = {
    { &QsciLexer::staticMetaObject, qt_meta_stringdata_QsciLexerCPP,
      qt_meta_data_QsciLexerCPP, 0 }
};

const QMetaObject *QsciLexerCPP::metaObject() const
{
    return &staticMetaObject;
}

void *QsciLexerCPP::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciLexerCPP))
	return static_cast<void*>(const_cast< QsciLexerCPP*>(this));
    return QsciLexer::qt_metacast(_clname);
}

int QsciLexerCPP::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QsciLexer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setFoldAtElse((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: setFoldComments((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: setFoldCompact((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: setFoldPreprocessor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: setStylePreprocessor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 5;
    }
    return _id;
}
