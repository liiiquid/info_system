/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../info_system_server/server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_server_t {
    QByteArrayData data[10];
    char stringdata0[134];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_server_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_server_t qt_meta_stringdata_server = {
    {
QT_MOC_LITERAL(0, 0, 6), // "server"
QT_MOC_LITERAL(1, 7, 12), // "main_process"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 14), // "print_msg_page"
QT_MOC_LITERAL(4, 36, 21), // "print_server_info_log"
QT_MOC_LITERAL(5, 58, 2), // "id"
QT_MOC_LITERAL(6, 61, 15), // "update_msg_list"
QT_MOC_LITERAL(7, 77, 17), // "update_online_num"
QT_MOC_LITERAL(8, 95, 15), // "print_info_page"
QT_MOC_LITERAL(9, 111, 22) // "update_server_info_log"

    },
    "server\0main_process\0\0print_msg_page\0"
    "print_server_info_log\0id\0update_msg_list\0"
    "update_online_num\0print_info_page\0"
    "update_server_info_log"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_server[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x0a /* Public */,
       3,    1,   52,    2, 0x0a /* Public */,
       4,    1,   55,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    1,   60,    2, 0x0a /* Public */,
       9,    1,   63,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<server *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->main_process((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->print_msg_page((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->print_server_info_log((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->update_msg_list(); break;
        case 4: _t->update_online_num(); break;
        case 5: _t->print_info_page((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->update_server_info_log((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject server::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_server.data,
    qt_meta_data_server,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_server.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
