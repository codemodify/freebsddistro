/****************************************************************************
** Meta object code from reading C++ file 'menu.h'
**
** Created: Sat Dec 2 11:12:28 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "menu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'menu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_menu[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_menu[] = {
    "menu\0\0run_menu(QAction*)\0"
};

const QMetaObject menu::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_menu,
      qt_meta_data_menu, 0 }
};

const QMetaObject *menu::metaObject() const
{
    return &staticMetaObject;
}

void *menu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_menu))
	return static_cast<void*>(const_cast<menu*>(this));
    return QPushButton::qt_metacast(_clname);
}

int menu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: run_menu((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
