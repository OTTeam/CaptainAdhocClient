/****************************************************************************
** Meta object code from reading C++ file 'GestionClients.h'
**
** Created: Wed 7. Mar 11:57:26 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SimsProtocole/GestionClients.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GestionClients.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GestionClients[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   16,   15,   15, 0x05,
      50,   16,   15,   15, 0x05,
      84,   16,   15,   15, 0x05,
     121,   16,   15,   15, 0x05,
     160,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     185,   15,   15,   15, 0x08,
     208,   15,   15,   15, 0x08,
     235,   15,   15,   15, 0x08,
     255,   15,   15,   15, 0x08,
     297,  271,   15,   15, 0x08,
     354,   15,   15,   15, 0x08,
     385,   15,   15,   15, 0x08,
     404,   15,   15,   15, 0x08,
     429,   15,   15,   15, 0x08,
     454,  447,   15,   15, 0x08,
     479,   15,   15,   15, 0x08,
     502,   15,   15,   15, 0x08,
     527,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GestionClients[] = {
    "GestionClients\0\0,\0ClientUploadUpdate(Client*,int)\0"
    "ClientDownloadUpdate(Client*,int)\0"
    "ClientUploadSpeedUpdate(Client*,int)\0"
    "ClientDownloadSpeedUpdate(Client*,int)\0"
    "ClientNumberChanged(int)\0"
    "clientBytesAvailable()\0"
    "clientBytesWritten(qint64)\0"
    "clientReceived(int)\0clientSent(int)\0"
    "broadcasterAddress,routes\0"
    "newConnectionRequest(QHostAddress,QList<RoutesTableElt>)\0"
    "newConnectionDone(QTcpSocket*)\0"
    "clientDisconnect()\0clientConnectionFailed()\0"
    "clientConnected()\0client\0"
    "NewClientConfig(Client*)\0"
    "uploadSpeedUpdate(int)\0downloadSpeedUpdate(int)\0"
    "broadCastTrigger()\0"
};

const QMetaObject GestionClients::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GestionClients,
      qt_meta_data_GestionClients, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GestionClients::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GestionClients::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GestionClients::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GestionClients))
        return static_cast<void*>(const_cast< GestionClients*>(this));
    return QObject::qt_metacast(_clname);
}

int GestionClients::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ClientUploadUpdate((*reinterpret_cast< Client*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: ClientDownloadUpdate((*reinterpret_cast< Client*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: ClientUploadSpeedUpdate((*reinterpret_cast< Client*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: ClientDownloadSpeedUpdate((*reinterpret_cast< Client*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: ClientNumberChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: clientBytesAvailable(); break;
        case 6: clientBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 7: clientReceived((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: clientSent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: newConnectionRequest((*reinterpret_cast< QHostAddress(*)>(_a[1])),(*reinterpret_cast< QList<RoutesTableElt>(*)>(_a[2]))); break;
        case 10: newConnectionDone((*reinterpret_cast< QTcpSocket*(*)>(_a[1]))); break;
        case 11: clientDisconnect(); break;
        case 12: clientConnectionFailed(); break;
        case 13: clientConnected(); break;
        case 14: NewClientConfig((*reinterpret_cast< Client*(*)>(_a[1]))); break;
        case 15: uploadSpeedUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: downloadSpeedUpdate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: broadCastTrigger(); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void GestionClients::ClientUploadUpdate(Client * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GestionClients::ClientDownloadUpdate(Client * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GestionClients::ClientUploadSpeedUpdate(Client * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GestionClients::ClientDownloadSpeedUpdate(Client * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GestionClients::ClientNumberChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
