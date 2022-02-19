/****************************************************************************
** Meta object code from reading C++ file 't_socket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../info_system_client/t_network/t_socket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 't_socket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_t_socket_t {
    QByteArrayData data[10];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_t_socket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_t_socket_t qt_meta_stringdata_t_socket = {
    {
QT_MOC_LITERAL(0, 0, 8), // "t_socket"
QT_MOC_LITERAL(1, 9, 9), // "write_out"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 8), // "message*"
QT_MOC_LITERAL(4, 29, 11), // "write_out_2"
QT_MOC_LITERAL(5, 41, 18), // "QVector<message*>*"
QT_MOC_LITERAL(6, 60, 3), // "mgs"
QT_MOC_LITERAL(7, 64, 10), // "close_self"
QT_MOC_LITERAL(8, 75, 5), // "event"
QT_MOC_LITERAL(9, 81, 7) // "QEvent*"

    },
    "t_socket\0write_out\0\0message*\0write_out_2\0"
    "QVector<message*>*\0mgs\0close_self\0"
    "event\0QEvent*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_t_socket[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       4,    1,   37,    2, 0x0a /* Public */,
       7,    0,   40,    2, 0x0a /* Public */,
       8,    1,   41,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 9,    2,

       0        // eod
};

void t_socket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<t_socket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->write_out((*reinterpret_cast< message*(*)>(_a[1]))); break;
        case 1: _t->write_out_2((*reinterpret_cast< QVector<message*>*(*)>(_a[1]))); break;
        case 2: _t->close_self(); break;
        case 3: { bool _r = _t->event((*reinterpret_cast< QEvent*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject t_socket::staticMetaObject = { {
    &QTcpSocket::staticMetaObject,
    qt_meta_stringdata_t_socket.data,
    qt_meta_data_t_socket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *t_socket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *t_socket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_t_socket.stringdata0))
        return static_cast<void*>(this);
    return QTcpSocket::qt_metacast(_clname);
}

int t_socket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
