/****************************************************************************
** Meta object code from reading C++ file 'graphicscene.h'
**
** Created: Sun Dec 30 19:54:39 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-tp1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../graphicscene/graphicscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphicscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-tp1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GraphicsScene[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      27,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,   50,   14,   14, 0x0a,
      85,   14,   14,   14, 0x0a,
      98,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GraphicsScene[] = {
    "GraphicsScene\0\0objectToAdd\0"
    "sendObject(QByteArray)\0objectToSend\0"
    "addObject(QByteArray)\0slot_Clear()\0"
    "slot_SendSnapshot()\0"
};

const QMetaObject GraphicsScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_GraphicsScene,
      qt_meta_data_GraphicsScene, 0 }
};

const QMetaObject *GraphicsScene::metaObject() const
{
    return &staticMetaObject;
}

void *GraphicsScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GraphicsScene))
	return static_cast<void*>(const_cast< GraphicsScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int GraphicsScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendObject((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: addObject((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: slot_Clear(); break;
        case 3: slot_SendSnapshot(); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void GraphicsScene::sendObject(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
