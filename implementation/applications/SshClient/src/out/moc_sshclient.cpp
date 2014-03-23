/****************************************************************************
** Meta object code from reading C++ file 'sshclient.h'
**
** Created: Wed Dec 26 17:40:50 2007
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-tp1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sshclient/sshclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sshclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-tp1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SshClient[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      23,   10,   10,   10, 0x08,
      49,   10,   10,   10, 0x08,
      97,   10,   10,   10, 0x08,
     140,   10,   10,   10, 0x08,
     167,   10,   10,   10, 0x08,
     197,   10,   10,   10, 0x08,
     248,   10,   10,   10, 0x08,
     275,   10,   10,   10, 0x08,
     333,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SshClient[] = {
    "SshClient\0\0slot_Send()\0slot_ConnectionEcrypted()\0"
    "slot_ConnectionSateChanged(QSslSocket::SslMode)\0"
    "slot_ConnectionSslErrors(QList<QSslError>)\0"
    "slot_ConnectionConnected()\0"
    "slot_ConnectionDisconnected()\0"
    "slot_ConnectionError(QAbstractSocket::SocketError)\0"
    "slot_ConnectionHostFound()\0"
    "slot_ConnectionStateChanged(QAbstractSocket::SocketState)\0"
    "slot_ConnectionStatereadyRead()\0"
};

const QMetaObject SshClient::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SshClient,
      qt_meta_data_SshClient, 0 }
};

const QMetaObject *SshClient::metaObject() const
{
    return &staticMetaObject;
}

void *SshClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SshClient))
	return static_cast<void*>(const_cast< SshClient*>(this));
    return QWidget::qt_metacast(_clname);
}

int SshClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slot_Send(); break;
        case 1: slot_ConnectionEcrypted(); break;
        case 2: slot_ConnectionSateChanged((*reinterpret_cast< QSslSocket::SslMode(*)>(_a[1]))); break;
        case 3: slot_ConnectionSslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 4: slot_ConnectionConnected(); break;
        case 5: slot_ConnectionDisconnected(); break;
        case 6: slot_ConnectionError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 7: slot_ConnectionHostFound(); break;
        case 8: slot_ConnectionStateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 9: slot_ConnectionStatereadyRead(); break;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
