/****************************************************************************
** Meta object code from reading C++ file 'xwindow.h'
**
** Created: Sat Dec 2 11:13:19 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "xwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'xwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_xwindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      21,    8,    8,    8, 0x0a,
      32,    8,    8,    8, 0x0a,
      52,    8,    8,    8, 0x0a,
      58,    8,    8,    8, 0x0a,
      71,    8,    8,    8, 0x0a,
      79,    8,    8,    8, 0x0a,
      91,    8,    8,    8, 0x0a,
      99,    8,    8,    8, 0x0a,
     109,    8,    8,    8, 0x0a,
     122,    8,    8,    8, 0x0a,
     135,    8,    8,    8, 0x0a,
     150,    8,    8,    8, 0x0a,
     166,    8,    8,    8, 0x0a,
     191,    8,    8,    8, 0x0a,
     218,    8,    8,    8, 0x0a,
     249,    8,    8,    8, 0x0a,
     282,    8,    8,    8, 0x0a,
     314,    8,    8,    8, 0x0a,
     348,    8,    8,    8, 0x0a,
     378,    8,    8,    8, 0x0a,
     410,    8,    8,    8, 0x0a,
     434,    8,    8,    8, 0x0a,
     464,    8,    8,    8, 0x0a,
     495,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_xwindow[] = {
    "xwindow\0\0state(bool)\0wdestroy()\0focus_mouse_wlist()\0map()\0"
    "map_normal()\0raise()\0show_info()\0unmap()\0iconify()\0t_maximize()\0"
    "s_maximize()\0toggle_tiled()\0tile_maximize()\0press_move(QMouseEvent*)\0"
    "release_move(QMouseEvent*)\0press_leftresize(QMouseEvent*)\0"
    "release_leftresize(QMouseEvent*)\0press_rightresize(QMouseEvent*)\0"
    "release_rightresize(QMouseEvent*)\0press_midresize(QMouseEvent*)\0"
    "release_midresize(QMouseEvent*)\0move_move(QMouseEvent*)\0"
    "move_leftresize(QMouseEvent*)\0move_rightresize(QMouseEvent*)\0"
    "move_midresize(QMouseEvent*)\0"
};

const QMetaObject xwindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_xwindow,
      qt_meta_data_xwindow, 0 }
};

const QMetaObject *xwindow::metaObject() const
{
    return &staticMetaObject;
}

void *xwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_xwindow))
	return static_cast<void*>(const_cast<xwindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int xwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: state((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: wdestroy(); break;
        case 2: focus_mouse_wlist(); break;
        case 3: map(); break;
        case 4: map_normal(); break;
        case 5: raise(); break;
        case 6: show_info(); break;
        case 7: unmap(); break;
        case 8: iconify(); break;
        case 9: t_maximize(); break;
        case 10: s_maximize(); break;
        case 11: toggle_tiled(); break;
        case 12: tile_maximize(); break;
        case 13: press_move((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 14: release_move((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 15: press_leftresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 16: release_leftresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 17: press_rightresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 18: release_rightresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 19: press_midresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 20: release_midresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 21: move_move((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 22: move_leftresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 23: move_rightresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 24: move_midresize((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        }
        _id -= 25;
    }
    return _id;
}
