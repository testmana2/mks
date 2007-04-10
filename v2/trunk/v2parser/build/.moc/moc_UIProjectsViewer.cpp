/****************************************************************************
** Meta object code from reading C++ file 'UIProjectsViewer.h'
**
** Created: Tue Apr 10 19:59:15 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/ui/UIProjectsViewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UIProjectsViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_UIProjectsViewer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_UIProjectsViewer[] = {
    "UIProjectsViewer\0\0on_pbSettings_clicked()\0"
};

const QMetaObject UIProjectsViewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_UIProjectsViewer,
      qt_meta_data_UIProjectsViewer, 0 }
};

const QMetaObject *UIProjectsViewer::metaObject() const
{
    return &staticMetaObject;
}

void *UIProjectsViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UIProjectsViewer))
	return static_cast<void*>(const_cast< UIProjectsViewer*>(this));
    if (!strcmp(_clname, "Ui::UIProjectsViewer"))
	return static_cast< Ui::UIProjectsViewer*>(const_cast< UIProjectsViewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int UIProjectsViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pbSettings_clicked(); break;
        }
        _id -= 1;
    }
    return _id;
}
