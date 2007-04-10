/****************************************************************************
** Meta object code from reading C++ file 'UIQMakeProjectSettings.h'
**
** Created: Tue Apr 10 19:59:17 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/ui/UIQMakeProjectSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UIQMakeProjectSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_UIQMakeProjectSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x09,
      57,   23,   23,   23, 0x09,
      76,   23,   23,   23, 0x09,
      96,   23,   23,   23, 0x09,
     118,   23,   23,   23, 0x09,
     139,   23,   23,   23, 0x09,
     157,   23,   23,   23, 0x09,
     177,   23,   23,   23, 0x09,
     186,   23,   23,   23, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_UIQMakeProjectSettings[] = {
    "UIQMakeProjectSettings\0\0"
    "on_tvScopes_clicked(QModelIndex)\0"
    "on_tbAdd_clicked()\0on_tbEdit_clicked()\0"
    "on_tbRemove_clicked()\0on_tbClear_clicked()\0"
    "on_tbUp_clicked()\0on_tbDown_clicked()\0"
    "accept()\0reject()\0"
};

const QMetaObject UIQMakeProjectSettings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_UIQMakeProjectSettings,
      qt_meta_data_UIQMakeProjectSettings, 0 }
};

const QMetaObject *UIQMakeProjectSettings::metaObject() const
{
    return &staticMetaObject;
}

void *UIQMakeProjectSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UIQMakeProjectSettings))
	return static_cast<void*>(const_cast< UIQMakeProjectSettings*>(this));
    if (!strcmp(_clname, "Ui::UIQMakeProjectSettings"))
	return static_cast< Ui::UIQMakeProjectSettings*>(const_cast< UIQMakeProjectSettings*>(this));
    return QDialog::qt_metacast(_clname);
}

int UIQMakeProjectSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_tvScopes_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: on_tbAdd_clicked(); break;
        case 2: on_tbEdit_clicked(); break;
        case 3: on_tbRemove_clicked(); break;
        case 4: on_tbClear_clicked(); break;
        case 5: on_tbUp_clicked(); break;
        case 6: on_tbDown_clicked(); break;
        case 7: accept(); break;
        case 8: reject(); break;
        }
        _id -= 9;
    }
    return _id;
}
