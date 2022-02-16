/****************************************************************************
** Meta object code from reading C++ file 'mthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../info_system_client/thread_about/mthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mthread_t {
    QByteArrayData data[8];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mthread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mthread_t qt_meta_stringdata_mthread = {
    {
QT_MOC_LITERAL(0, 0, 7), // "mthread"
QT_MOC_LITERAL(1, 8, 7), // "read_ok"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 10), // "process_ok"
QT_MOC_LITERAL(4, 28, 14), // "connect_failed"
QT_MOC_LITERAL(5, 43, 4), // "read"
QT_MOC_LITERAL(6, 48, 7), // "process"
QT_MOC_LITERAL(7, 56, 11) // "quit_thread"

    },
    "mthread\0read_ok\0\0process_ok\0connect_failed\0"
    "read\0process\0quit_thread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mthread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,
       4,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void mthread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mthread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->read_ok(); break;
        case 1: _t->process_ok(); break;
        case 2: _t->connect_failed(); break;
        case 3: _t->read(); break;
        case 4: _t->process(); break;
        case 5: _t->quit_thread(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (mthread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mthread::read_ok)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (mthread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mthread::process_ok)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (mthread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mthread::connect_failed)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject mthread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_mthread.data,
    qt_meta_data_mthread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mthread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mthread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mthread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int mthread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void mthread::read_ok()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void mthread::process_ok()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void mthread::connect_failed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
