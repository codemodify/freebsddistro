/****************************************************************************
** Meta object code from reading C++ file 'winfo.h'
**
** Created: Sat Dec 2 11:13:08 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "winfo.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'winfo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_winfo[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      27,    6,    6,    6, 0x08,
      47,    6,    6,    6, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_winfo[] = {
    "winfo\0\0release_temporary()\0release_permanent()\0release_cancel()\0"
};

const QMetaObject winfo::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_winfo,
      qt_meta_data_winfo, 0 }
};

const QMetaObject *winfo::metaObject() const
{
    return &staticMetaObject;
}

void *winfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_winfo))
	return static_cast<void*>(const_cast<winfo*>(this));
    return QWidget::qt_metacast(_clname);
}

int winfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: release_temporary(); break;
        case 1: release_permanent(); break;
        case 2: release_cancel(); break;
        }
        _id -= 3;
    }
    return _id;
}
