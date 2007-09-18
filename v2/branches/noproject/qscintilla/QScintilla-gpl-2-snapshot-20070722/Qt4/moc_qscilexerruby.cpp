/****************************************************************************
** Meta object code from reading C++ file 'qscilexerruby.h'
**
** Created: Sat Sep 15 16:32:13 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Qsci/qscilexerruby.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscilexerruby.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciLexerRuby[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QsciLexerRuby[] = {
    "QsciLexerRuby\0"
};

const QMetaObject QsciLexerRuby::staticMetaObject = {
    { &QsciLexer::staticMetaObject, qt_meta_stringdata_QsciLexerRuby,
      qt_meta_data_QsciLexerRuby, 0 }
};

const QMetaObject *QsciLexerRuby::metaObject() const
{
    return &staticMetaObject;
}

void *QsciLexerRuby::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciLexerRuby))
	return static_cast<void*>(const_cast< QsciLexerRuby*>(this));
    return QsciLexer::qt_metacast(_clname);
}

int QsciLexerRuby::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QsciLexer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
