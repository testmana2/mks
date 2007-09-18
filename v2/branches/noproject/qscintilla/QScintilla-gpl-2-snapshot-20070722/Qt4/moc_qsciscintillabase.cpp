/****************************************************************************
** Meta object code from reading C++ file 'qsciscintillabase.h'
**
** Created: Sat Sep 15 16:31:50 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Qsci/qsciscintillabase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsciscintillabase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QsciScintillaBase[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      23,   19,   18,   18, 0x05,
      64,   45,   18,   18, 0x05,
     100,   18,   18,   18, 0x05,
     124,  114,   18,   18, 0x05,
     156,  146,   18,   18, 0x05,
     199,  175,   18,   18, 0x05,
     231,  228,   18,   18, 0x05,
     257,  228,   18,   18, 0x05,
     304,  285,   18,   18, 0x05,
     330,  285,   18,   18, 0x05,
     362,  285,   18,   18, 0x05,
     390,  285,   18,   18, 0x05,
     420,  228,   18,   18, 0x05,
     479,  453,   18,   18, 0x05,
     516,  508,   18,   18, 0x05,
     570,   18,   18,   18, 0x05,
     594,  592,   18,   18, 0x05,
     617,   18,   18,   18, 0x05,
     631,   18,   18,   18, 0x05,
     651,   18,   18,   18, 0x05,
     683,  674,   18,   18, 0x05,
     704,   18,   18,   18, 0x05,
     719,  592,   18,   18, 0x05,
     758,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     769,   18,   18,   18, 0x08,
     789,  783,   18,   18, 0x08,
     804,  783,   18,   18, 0x08,
     819,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QsciScintillaBase[] = {
    "QsciScintillaBase\0\0yes\0QSCN_SELCHANGED(bool)\0"
    "selection,position\0"
    "SCN_AUTOCSELECTION(const char*,int)\0"
    "SCEN_CHANGE()\0direction\0SCN_CALLTIPCLICK(int)\0"
    "charadded\0SCN_CHARADDED(int)\0"
    "position,line,modifiers\0"
    "SCN_DOUBLECLICK(int,int,int)\0,,\0"
    "SCN_DWELLEND(int,int,int)\0"
    "SCN_DWELLSTART(int,int,int)\0"
    "position,modifiers\0SCN_HOTSPOTCLICK(int,int)\0"
    "SCN_HOTSPOTDOUBLECLICK(int,int)\0"
    "SCN_INDICATORCLICK(int,int)\0"
    "SCN_INDICATORRELEASE(int,int)\0"
    "SCN_MACRORECORD(uint,ulong,long)\0"
    "position,modifiers,margin\0"
    "SCN_MARGINCLICK(int,int,int)\0,,,,,,,\0"
    "SCN_MODIFIED(int,int,const char*,int,int,int,int,int)\0"
    "SCN_MODIFYATTEMPTRO()\0,\0SCN_NEEDSHOWN(int,int)\0"
    "SCN_PAINTED()\0SCN_SAVEPOINTLEFT()\0"
    "SCN_SAVEPOINTREACHED()\0position\0"
    "SCN_STYLENEEDED(int)\0SCN_UPDATEUI()\0"
    "SCN_USERLISTSELECTION(const char*,int)\0"
    "SCN_ZOOM()\0handleTimer()\0value\0"
    "handleVSb(int)\0handleHSb(int)\0"
    "handleSelection()\0"
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
	return static_cast<void*>(const_cast< QsciScintillaBase*>(this));
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
        case 5: SCN_DOUBLECLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: SCN_DWELLEND((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: SCN_DWELLSTART((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: SCN_HOTSPOTCLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: SCN_HOTSPOTDOUBLECLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: SCN_INDICATORCLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: SCN_INDICATORRELEASE((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: SCN_MACRORECORD((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2])),(*reinterpret_cast< long(*)>(_a[3]))); break;
        case 13: SCN_MARGINCLICK((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 14: SCN_MODIFIED((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const char*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8]))); break;
        case 15: SCN_MODIFYATTEMPTRO(); break;
        case 16: SCN_NEEDSHOWN((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: SCN_PAINTED(); break;
        case 18: SCN_SAVEPOINTLEFT(); break;
        case 19: SCN_SAVEPOINTREACHED(); break;
        case 20: SCN_STYLENEEDED((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: SCN_UPDATEUI(); break;
        case 22: SCN_USERLISTSELECTION((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 23: SCN_ZOOM(); break;
        case 24: handleTimer(); break;
        case 25: handleVSb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: handleHSb((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: handleSelection(); break;
        }
        _id -= 28;
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
void QsciScintillaBase::SCN_DOUBLECLICK(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
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
void QsciScintillaBase::SCN_INDICATORCLICK(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QsciScintillaBase::SCN_INDICATORRELEASE(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void QsciScintillaBase::SCN_MACRORECORD(unsigned int _t1, unsigned long _t2, long _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void QsciScintillaBase::SCN_MARGINCLICK(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void QsciScintillaBase::SCN_MODIFIED(int _t1, int _t2, const char * _t3, int _t4, int _t5, int _t6, int _t7, int _t8)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)), const_cast<void*>(reinterpret_cast<const void*>(&_t8)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void QsciScintillaBase::SCN_MODIFYATTEMPTRO()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void QsciScintillaBase::SCN_NEEDSHOWN(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void QsciScintillaBase::SCN_PAINTED()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void QsciScintillaBase::SCN_SAVEPOINTLEFT()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void QsciScintillaBase::SCN_SAVEPOINTREACHED()
{
    QMetaObject::activate(this, &staticMetaObject, 19, 0);
}

// SIGNAL 20
void QsciScintillaBase::SCN_STYLENEEDED(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void QsciScintillaBase::SCN_UPDATEUI()
{
    QMetaObject::activate(this, &staticMetaObject, 21, 0);
}

// SIGNAL 22
void QsciScintillaBase::SCN_USERLISTSELECTION(const char * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void QsciScintillaBase::SCN_ZOOM()
{
    QMetaObject::activate(this, &staticMetaObject, 23, 0);
}
