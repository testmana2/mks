/****************************************************************************
** Meta object code from reading C++ file 'qscilexercsharp.h'
**
** Created: Thu Oct 26 22:14:34 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qscilexercsharp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscilexercsharp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciLexerCSharp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QsciLexerCSharp[] = {
    "QsciLexerCSharp\0"
};

const QMetaObject QsciLexerCSharp::staticMetaObject = {
    { &QsciLexerCPP::staticMetaObject, qt_meta_stringdata_QsciLexerCSharp,
      qt_meta_data_QsciLexerCSharp, 0 }
};

const QMetaObject *QsciLexerCSharp::metaObject() const
{
    return &staticMetaObject;
}

void *QsciLexerCSharp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciLexerCSharp))
	return static_cast<void*>(const_cast<QsciLexerCSharp*>(this));
    return QsciLexerCPP::qt_metacast(_clname);
}

int QsciLexerCSharp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QsciLexerCPP::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
