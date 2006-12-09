/****************************************************************************
** Meta object code from reading C++ file 'qsciscintillabase.h'
**
** Created: Thu Oct 26 22:14:27 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qsciscintillabase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsciscintillabase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciScintillaBase[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      23,   19,   18,   18, 0x05,
      64,   45,   18,   18, 0x05,
     100,   18,   18,   18, 0x05,
     124,  114,   18,   18, 0x05,
     156,  146,   18,   18, 0x05,
     189,  175,   18,   18, 0x05,
     217,  214,   18,   18, 0x05,
     243,  214,   18,   18, 0x05,
     290,  271,   18,   18, 0x05,
     316,  271,   18,   18, 0x05,
     348,  214,   18,   18, 0x05,
     407,  381,   18,   18, 0x05,
     444,  436,   18,   18, 0x05,
     498,   18,   18,   18, 0x05,
     522,  520,   18,   18, 0x05,
     545,   18,   18,   18, 0x05,
     559,   18,   18,   18, 0x05,
     579,   18,   18,   18, 0x05,
     611,  602,   18,   18, 0x05,
     632,   18,   18,   18, 0x05,
     647,  520,   18,   18, 0x05,
     686,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     697,   18,   18,   18, 0x08,
     717,  711,   18,   18, 0x08,
     732,  711,   18,   18, 0x08,
     747,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QsciScintillaBase[] = {
    "QsciScintillaBase\0\0yes\0QSCN_SELCHANGED(bool)\0selection,position\0"
    "SCN_AUTOCSELECTION(const char*,int)\0SCEN_CHANGE()\0direction\0"
    "SCN_CALLTIPCLICK(int)\0charadded\0SCN_CHARADDED(int)\0position,line\0"
    "SCN_DOUBLECLICK(int,int)\0,,\0SCN_DWELLEND(int,int,int)\0"
    "SCN_DWELLSTART(int,int,int)\0position,modifiers\0"
    "SCN_HOTSPOTCLICK(int,int)\0SCN_HOTSPOTDOUBLECLICK(int,int)\0"
    "SCN_MACRORECORD(uint,ulong,long)\0position,modifiers,margin\0"
    "SCN_MARGINCLICK(int,int,int)\0,,,,,,,\0"
    "SCN_MODIFIED(int,int,const char*,int,int,int,int,int)\0"
    "SCN_MODIFYATTEMPTRO()\0,\0SCN_NEEDSHOWN(int,int)\0SCN_PAINTED()\0"
    "SCN_SAVEPOINTLEFT()\0SCN_SAVEPOINTREACHED()\0position\0"
    "SCN_STYLENEEDED(int)\0SCN_UPDATEUI()\0"
    "SCN_USERLISTSELECTION(const char*,int)\0SCN_ZOOM()\0handleTimer()\0"
    "value\0handleVSb(int)\0handleHSb(int)\0handleSelection()\0"
};

const QMetaObject QsciScintillaBase::staticMetaObject = {
    { &QAbstractScrollArea::staticMetaObject, qt_meta_stringdata_QsciScintillaBase,
      qt_meta_data_QsciScintillaBase, 0 }
};

const QMetaObject *QsciScintillaBase::metaObject() const
{
    return &staticMetaObject;
}

void *QsciScintillaBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QsciScintillaBase))
	return static_cast<void*>(const_cast<QsciScintillaBase*>(this));
    return QAbstractScrollArea::qt_metacast(_clname);
}

int QsciScintillaBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: QSCN_SELCHANGED((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: SCN_AUTOCSELECTION((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: SCEN_CHANGE(); break;
        case 3: SCN_CALLTIPCLICK((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: SCN_CHARADDED((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: SCN_DOUBLECLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: SCN_DWELLEND((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: SCN_DWELLSTART((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: SCN_HOTSPOTCLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: SCN_HOTSPOTDOUBLECLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: SCN_MACRORECORD((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2])),(*reinterpret_cast< long(*)>(_a[3]))); break;
        case 11: SCN_MARGINCLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 12: SCN_MODIFIED((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const char*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 13: SCN_MODIFYATTEMPTRO(); break;
        case 14: SCN_NEEDSHOWN((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: SCN_PAINTED(); break;
        case 16: SCN_SAVEPOINTLEFT(); break;
        case 17: SCN_SAVEPOINTREACHED(); break;
        case 18: SCN_STYLENEEDED((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: SCN_UPDATEUI(); break;
        case 20: SCN_USERLISTSELECTION((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 21: SCN_ZOOM(); break;
        case 22: handleTimer(); break;
        case 23: handleVSb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: handleHSb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: handleSelection(); break;
        }
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void QsciScintillaBase::QSCN_SELCHANGED(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QsciScintillaBase::SCN_AUTOCSELECTION(const char * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QsciScintillaBase::SCEN_CHANGE()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QsciScintillaBase::SCN_CALLTIPCLICK(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QsciScintillaBase::SCN_CHARADDED(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QsciScintillaBase::SCN_DOUBLECLICK(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QsciScintillaBase::SCN_DWELLEND(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QsciScintillaBase::SCN_DWELLSTART(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QsciScintillaBase::SCN_HOTSPOTCLICK(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void QsciScintillaBase::SCN_HOTSPOTDOUBLECLICK(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QsciScintillaBase::SCN_MACRORECORD(unsigned int _t1, unsigned long _t2, long _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QsciScintillaBase::SCN_MARGINCLICK(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void QsciScintillaBase::SCN_MODIFIED(int _t1, int _t2, const char * _t3, int _t4, int _t5, int _t6, int _t7, int _t8)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void QsciScintillaBase::SCN_MODIFYATTEMPTRO()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void QsciScintillaBase::SCN_NEEDSHOWN(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void QsciScintillaBase::SCN_PAINTED()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void QsciScintillaBase::SCN_SAVEPOINTLEFT()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void QsciScintillaBase::SCN_SAVEPOINTREACHED()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void QsciScintillaBase::SCN_STYLENEEDED(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void QsciScintillaBase::SCN_UPDATEUI()
{
    QMetaObject::activate(this, &staticMetaObject, 19, 0);
}

// SIGNAL 20
void QsciScintillaBase::SCN_USERLISTSELECTION(const char * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void QsciScintillaBase::SCN_ZOOM()
{
    QMetaObject::activate(this, &staticMetaObject, 21, 0);
}
