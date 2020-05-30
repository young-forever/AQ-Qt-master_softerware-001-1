/****************************************************************************
** Meta object code from reading C++ file 'image_save_thread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AQ001-1/image_save_thread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'image_save_thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_image_save_thread_t {
    QByteArrayData data[3];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_image_save_thread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_image_save_thread_t qt_meta_stringdata_image_save_thread = {
    {
QT_MOC_LITERAL(0, 0, 17), // "image_save_thread"
QT_MOC_LITERAL(1, 18, 15), // "stopImmediately"
QT_MOC_LITERAL(2, 34, 0) // ""

    },
    "image_save_thread\0stopImmediately\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_image_save_thread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void image_save_thread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        image_save_thread *_t = static_cast<image_save_thread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stopImmediately(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject image_save_thread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_image_save_thread.data,
      qt_meta_data_image_save_thread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *image_save_thread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *image_save_thread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_image_save_thread.stringdata0))
        return static_cast<void*>(const_cast< image_save_thread*>(this));
    return QThread::qt_metacast(_clname);
}

int image_save_thread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
