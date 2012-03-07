/****************************************************************************
** Meta object code from reading C++ file 'Client.h'
**
** Created: Wed 7. Mar 11:57:27 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SimsProtocole/Client.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Client[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      21,    7,    7,    7, 0x05,
      42,    7,    7,    7, 0x05,
      67,    7,    7,    7, 0x05,
      92,    7,    7,    7, 0x05,
     115,    7,    7,    7, 0x05,
     127,    7,    7,    7, 0x05,
     141,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
     156,    7,    7,    7, 0x0a,
     175,    7,    7,    7, 0x0a,
     199,    7,    7,    7, 0x08,
     216,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Client[] = {
    "Client\0\0NewData(int)\0BytesSentUpdate(int)\0"
    "BytesReceivedUpdate(int)\0"
    "DownloadSpeedUpdate(int)\0"
    "UploadSpeedUpdate(int)\0Connected()\0"
    "SocketError()\0Disconnected()\0"
    "newBytesReceived()\0newBytesWritten(qint64)\0"
    "dlSpeedMeasure()\0ulSpeedMeasure()\0"
};

const QMetaObject Client::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Client,
      qt_meta_data_Client, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Client::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Client::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Client))
        return static_cast<void*>(const_cast< Client*>(this));
    return QObject::qt_metacast(_clname);
}

int Client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: NewData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: BytesSentUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: BytesReceivedUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: DownloadSpeedUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: UploadSpeedUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: Connected(); break;
        case 6: SocketError(); break;
        case 7: Disconnected(); break;
        case 8: newBytesReceived(); break;
        case 9: newBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 10: dlSpeedMeasure(); break;
        case 11: ulSpeedMeasure(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Client::NewData(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Client::BytesSentUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Client::BytesReceivedUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Client::DownloadSpeedUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Client::UploadSpeedUpdate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Client::Connected()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void Client::SocketError()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Client::Disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
QT_END_MOC_NAMESPACE
