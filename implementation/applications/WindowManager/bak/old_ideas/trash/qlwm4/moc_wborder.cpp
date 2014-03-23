/****************************************************************************
** Meta object code from reading C++ file 'wborder.h'
**
** Created: Sat Dec 2 11:13:02 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "wborder.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wborder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_wframe[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      28,    7,    7,    7, 0x05,
      36,    7,    7,    7, 0x05,
      58,    7,    7,    7, 0x05,
      83,    7,    7,    7, 0x05,
     108,    7,    7,    7, 0x05,
     121,    7,    7,    7, 0x05,
     135,    7,    7,    7, 0x05,
     162,    7,    7,    7, 0x05,
     178,    7,    7,    7, 0x05,
     190,    7,    7,    7, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_wframe[] = {
    "wframe\0\0press(QMouseEvent*)\0press()\0release(QMouseEvent*)\0"
    "mouse_move(QMouseEvent*)\0left_press(QMouseEvent*)\0left_press()\0"
    "right_press()\0left_release(QMouseEvent*)\0right_release()\0mid_press()\0"
    "mid_release()\0"
};

const QMetaObject wframe::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_wframe,
      qt_meta_data_wframe, 0 }
};

const QMetaObject *wframe::metaObject() const
{
    return &staticMetaObject;
}

void *wframe::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wframe))
	return static_cast<void*>(const_cast<wframe*>(this));
    return QLabel::qt_metacast(_clname);
}

int wframe::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: press((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: press(); break;
        case 2: release((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: mouse_move((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: left_press((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: left_press(); break;
        case 6: right_press(); break;
        case 7: left_release((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 8: right_release(); break;
        case 9: mid_press(); break;
        case 10: mid_release(); break;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void wframe::press(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void wframe::press()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void wframe::release(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void wframe::mouse_move(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void wframe::left_press(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void wframe::left_press()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void wframe::right_press()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void wframe::left_release(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void wframe::right_release()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void wframe::mid_press()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void wframe::mid_release()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}
