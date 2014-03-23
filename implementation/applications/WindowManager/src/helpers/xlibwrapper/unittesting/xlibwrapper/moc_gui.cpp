/****************************************************************************
** Meta object code from reading C++ file 'gui.h'
**
** Created: Fri Mar 30 11:56:56 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gui/gui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Gui[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      18,    4,    4,    4, 0x08,
      33,    4,    4,    4, 0x08,
      46,    4,    4,    4, 0x08,
      67,    4,    4,    4, 0x08,
      88,    4,    4,    4, 0x08,
     101,    4,    4,    4, 0x08,
     115,    4,    4,    4, 0x08,
     134,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Gui[] = {
    "Gui\0\0setDisplay()\0createWindow()\0"
    "showWindow()\0setWindowTitleText()\0"
    "getWindowTitleText()\0hideWindow()\0"
    "focusWindow()\0bringWindowOnTop()\0"
    "sendWindowToBack()\0"
};

const QMetaObject Gui::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Gui,
      qt_meta_data_Gui, 0 }
};

const QMetaObject *Gui::metaObject() const
{
    return &staticMetaObject;
}

void *Gui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Gui))
	return static_cast<void*>(const_cast<Gui*>(this));
    if (!strcmp(_clname, "Ui::Form"))
	return static_cast<Ui::Form*>(const_cast<Gui*>(this));
    return QWidget::qt_metacast(_clname);
}

int Gui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setDisplay(); break;
        case 1: createWindow(); break;
        case 2: showWindow(); break;
        case 3: setWindowTitleText(); break;
        case 4: getWindowTitleText(); break;
        case 5: hideWindow(); break;
        case 6: focusWindow(); break;
        case 7: bringWindowOnTop(); break;
        case 8: sendWindowToBack(); break;
        }
        _id -= 9;
    }
    return _id;
}
