/****************************************************************************
** Meta object code from reading C++ file 'CLIToolPlugin.h'
**
** Created: Sun Sep 16 15:01:51 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../monkey/src/pluginsmanager/CLIToolPlugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CLIToolPlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_CLIToolPlugin[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_CLIToolPlugin[] = {
    "CLIToolPlugin\0"
};

const QMetaObject CLIToolPlugin::staticMetaObject = {
    { &BasePlugin::staticMetaObject, qt_meta_stringdata_CLIToolPlugin,
      qt_meta_data_CLIToolPlugin, 0 }
};

const QMetaObject *CLIToolPlugin::metaObject() const
{
    return &staticMetaObject;
}

void *CLIToolPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLIToolPlugin))
	return static_cast<void*>(const_cast< CLIToolPlugin*>(this));
    return BasePlugin::qt_metacast(_clname);
}

int CLIToolPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BasePlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
