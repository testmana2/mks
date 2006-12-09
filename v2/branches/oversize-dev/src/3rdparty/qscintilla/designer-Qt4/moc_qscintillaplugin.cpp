/****************************************************************************
** Meta object code from reading C++ file 'qscintillaplugin.h'
**
** Created: Tue Oct 31 13:55:48 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qscintillaplugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscintillaplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_QScintillaPlugin[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_QScintillaPlugin[] = {
    "QScintillaPlugin\0"
};

const QMetaObject QScintillaPlugin::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QScintillaPlugin,
      qt_meta_data_QScintillaPlugin, 0 }
};

const QMetaObject *QScintillaPlugin::metaObject() const
{
    return &staticMetaObject;
}

void *QScintillaPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QScintillaPlugin))
	return static_cast<void*>(const_cast<QScintillaPlugin*>(this));
    if (!strcmp(_clname, "QDesignerCustomWidgetInterface"))
	return static_cast<QDesignerCustomWidgetInterface*>(const_cast<QScintillaPlugin*>(this));
    if (!strcmp(_clname, "com.trolltech.Qt.Designer.CustomWidget"))
	return static_cast<QDesignerCustomWidgetInterface*>(const_cast<QScintillaPlugin*>(this));
    return QObject::qt_metacast(_clname);
}

int QScintillaPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
