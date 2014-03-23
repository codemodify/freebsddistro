/****************************************************************************
** Meta object code from reading C++ file 'collaborativeschema.h'
**
** Created: Sun Dec 30 19:54:30 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-tp1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../collaborativeschema/collaborativeschema.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'collaborativeschema.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-tp1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CollaborativeSchema[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      34,   21,   20,   20, 0x05,
      60,   50,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,   21,   20,   20, 0x08,
     116,   20,   20,   20, 0x08,
     131,   20,   20,   20, 0x08,
     158,   20,   20,   20, 0x08,
     188,   20,   20,   20, 0x08,
     228,  215,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CollaborativeSchema[] = {
    "CollaborativeSchema\0\0statusString\0"
    "status(QString)\0byteArray\0"
    "objectAvailable(QByteArray)\0"
    "slot_StatusChanged(QString)\0slot_Connect()\0"
    "slot_ConnectionConnected()\0"
    "slot_ConnectionDisconnected()\0"
    "slot_ConnectionReadyRead()\0objectToSend\0"
    "slot_SendSchemaToPeers(QByteArray)\0"
};

const QMetaObject CollaborativeSchema::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CollaborativeSchema,
      qt_meta_data_CollaborativeSchema, 0 }
};

const QMetaObject *CollaborativeSchema::metaObject() const
{
    return &staticMetaObject;
}

void *CollaborativeSchema::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CollaborativeSchema))
	return static_cast<void*>(const_cast< CollaborativeSchema*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CollaborativeSchema::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: status((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: objectAvailable((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: slot_StatusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: slot_Connect(); break;
        case 4: slot_ConnectionConnected(); break;
        case 5: slot_ConnectionDisconnected(); break;
        case 6: slot_ConnectionReadyRead(); break;
        case 7: slot_SendSchemaToPeers((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CollaborativeSchema::status(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CollaborativeSchema::objectAvailable(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
