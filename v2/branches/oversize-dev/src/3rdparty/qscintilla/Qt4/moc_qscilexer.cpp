/****************************************************************************
** Meta object code from reading C++ file 'qscilexer.h'
**
** Created: Thu Oct 26 22:14:30 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qscilexer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscilexer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciLexer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   11,   10,   10, 0x05,
      60,   44,   10,   10, 0x05,
      93,   85,   10,   10, 0x05,
     116,   11,   10,   10, 0x05,
     150,  141,   10,   10, 0x05,
     191,   10,   10,   10, 0x05,
     214,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     254,  238,   10,   10, 0x0a,
     278,   11,   10,   10, 0x0a,
     301,  299,   10,   10, 0x2a,
     320,  318,   10,   10, 0x0a,
     342,  299,   10,   10, 0x0a,
     366,  299,   10,   10, 0x0a,
     404,  390,   10,   10, 0x0a,
     433,  425,   10,   10, 0x2a,
     450,   85,   10,   10, 0x0a,
     469,  318,   10,   10, 0x2a,
     484,   11,   10,   10, 0x0a,
     505,  299,   10,   10, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_QsciLexer[] = {
    "QsciLexer\0\0c,style\0colorChanged(QColor,int)\0eolfilled,style\0"
    "eolFillChanged(bool,int)\0f,style\0fontChanged(QFont,int)\0"
    "paperChanged(QColor,int)\0prop,val\0"
    "propertyChanged(const char*,const char*)\0apiProcessingStarted()\0"
    "apiProcessingFinished()\0autoindentstyle\0setAutoIndentStyle(int)\0"
    "setColor(QColor,int)\0c\0setColor(QColor)\0f\0setDefaultFont(QFont)\0"
    "setDefaultColor(QColor)\0setDefaultPaper(QColor)\0eoffill,style\0"
    "setEolFill(bool,int)\0eoffill\0setEolFill(bool)\0setFont(QFont,int)\0"
    "setFont(QFont)\0setPaper(QColor,int)\0setPaper(QColor)\0"
};

const QMetaObject QsciLexer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QsciLexer,
      qt_meta_data_QsciLexer, 0 }
};

const QMetaObject *QsciLexer::metaObject() const
{
    return &staticMetaObject;
}

void *QsciLexer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciLexer))
	return static_cast<void*>(const_cast<QsciLexer*>(this));
    return QObject::qt_metacast(_clname);
}

int QsciLexer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: colorChanged((*reinterpret_cast< const QColor(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: eolFillChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: fontChanged((*reinterpret_cast< const QFont(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: paperChanged((*reinterpret_cast< const QColor(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: propertyChanged((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< const char*(*)>(_a[2]))); break;
        case 5: apiProcessingStarted(); break;
        case 6: apiProcessingFinished(); break;
        case 7: setAutoIndentStyle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: setColor((*reinterpret_cast< const QColor(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: setColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 10: setDefaultFont((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 11: setDefaultColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 12: setDefaultPaper((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 13: setEolFill((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: setEolFill((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: setFont((*reinterpret_cast< const QFont(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: setFont((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 17: setPaper((*reinterpret_cast< const QColor(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: setPaper((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        }
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void QsciLexer::colorChanged(const QColor & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QsciLexer::eolFillChanged(bool _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QsciLexer::fontChanged(const QFont & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QsciLexer::paperChanged(const QColor & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QsciLexer::propertyChanged(const char * _t1, const char * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QsciLexer::apiProcessingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void QsciLexer::apiProcessingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
