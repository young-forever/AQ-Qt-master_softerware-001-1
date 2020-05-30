/****************************************************************************
** Meta object code from reading C++ file 'cameraworkthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AQ001-1/cameraworkthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cameraworkthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CameraWorkThread_t {
    QByteArrayData data[11];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CameraWorkThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CameraWorkThread_t qt_meta_stringdata_CameraWorkThread = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CameraWorkThread"
QT_MOC_LITERAL(1, 17, 10), // "show_image"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "unsigned char*"
QT_MOC_LITERAL(4, 44, 6), // "pImage"
QT_MOC_LITERAL(5, 51, 1), // "n"
QT_MOC_LITERAL(6, 53, 25), // "refresh_camera_cnt_signal"
QT_MOC_LITERAL(7, 79, 10), // "camera_cnt"
QT_MOC_LITERAL(8, 90, 15), // "image_path_save"
QT_MOC_LITERAL(9, 106, 5), // "path_"
QT_MOC_LITERAL(10, 112, 15) // "stopImmediately"

    },
    "CameraWorkThread\0show_image\0\0"
    "unsigned char*\0pImage\0n\0"
    "refresh_camera_cnt_signal\0camera_cnt\0"
    "image_path_save\0path_\0stopImmediately"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CameraWorkThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       6,    1,   39,    2, 0x06 /* Public */,
       8,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::UShort,    4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CameraWorkThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CameraWorkThread *_t = static_cast<CameraWorkThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->show_image((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< unsigned short(*)>(_a[2]))); break;
        case 1: _t->refresh_camera_cnt_signal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->image_path_save((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->stopImmediately(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CameraWorkThread::*_t)(unsigned char * , unsigned short );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraWorkThread::show_image)) {
                *result = 0;
            }
        }
        {
            typedef void (CameraWorkThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraWorkThread::refresh_camera_cnt_signal)) {
                *result = 1;
            }
        }
        {
            typedef void (CameraWorkThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CameraWorkThread::image_path_save)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject CameraWorkThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CameraWorkThread.data,
      qt_meta_data_CameraWorkThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CameraWorkThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraWorkThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CameraWorkThread.stringdata0))
        return static_cast<void*>(const_cast< CameraWorkThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CameraWorkThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CameraWorkThread::show_image(unsigned char * _t1, unsigned short _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CameraWorkThread::refresh_camera_cnt_signal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CameraWorkThread::image_path_save(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
