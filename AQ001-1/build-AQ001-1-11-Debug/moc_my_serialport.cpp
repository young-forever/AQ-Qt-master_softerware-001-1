/****************************************************************************
** Meta object code from reading C++ file 'my_serialport.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AQ001-1/my_serialport.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'my_serialport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_my_serialport_t {
    QByteArrayData data[9];
    char stringdata0[170];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_my_serialport_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_my_serialport_t qt_meta_stringdata_my_serialport = {
    {
QT_MOC_LITERAL(0, 0, 13), // "my_serialport"
QT_MOC_LITERAL(1, 14, 20), // "SR_ui_refresh_signal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 29), // "sensor_data_ui_refresh_signal"
QT_MOC_LITERAL(4, 66, 15), // "AQ_error_signal"
QT_MOC_LITERAL(5, 82, 27), // "AQ_error_Right_Limit_signal"
QT_MOC_LITERAL(6, 110, 26), // "AQ_error_Left_Limit_signal"
QT_MOC_LITERAL(7, 137, 16), // "serial_ok_signal"
QT_MOC_LITERAL(8, 154, 15) // "stopImmediately"

    },
    "my_serialport\0SR_ui_refresh_signal\0\0"
    "sensor_data_ui_refresh_signal\0"
    "AQ_error_signal\0AQ_error_Right_Limit_signal\0"
    "AQ_error_Left_Limit_signal\0serial_ok_signal\0"
    "stopImmediately"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_my_serialport[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    0,   51,    2, 0x06 /* Public */,
       5,    0,   52,    2, 0x06 /* Public */,
       6,    0,   53,    2, 0x06 /* Public */,
       7,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void my_serialport::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        my_serialport *_t = static_cast<my_serialport *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SR_ui_refresh_signal(); break;
        case 1: _t->sensor_data_ui_refresh_signal(); break;
        case 2: _t->AQ_error_signal(); break;
        case 3: _t->AQ_error_Right_Limit_signal(); break;
        case 4: _t->AQ_error_Left_Limit_signal(); break;
        case 5: _t->serial_ok_signal(); break;
        case 6: _t->stopImmediately(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (my_serialport::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&my_serialport::SR_ui_refresh_signal)) {
                *result = 0;
            }
        }
        {
            typedef void (my_serialport::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&my_serialport::sensor_data_ui_refresh_signal)) {
                *result = 1;
            }
        }
        {
            typedef void (my_serialport::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&my_serialport::AQ_error_signal)) {
                *result = 2;
            }
        }
        {
            typedef void (my_serialport::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&my_serialport::AQ_error_Right_Limit_signal)) {
                *result = 3;
            }
        }
        {
            typedef void (my_serialport::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&my_serialport::AQ_error_Left_Limit_signal)) {
                *result = 4;
            }
        }
        {
            typedef void (my_serialport::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&my_serialport::serial_ok_signal)) {
                *result = 5;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject my_serialport::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_my_serialport.data,
      qt_meta_data_my_serialport,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *my_serialport::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *my_serialport::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_my_serialport.stringdata0))
        return static_cast<void*>(const_cast< my_serialport*>(this));
    return QThread::qt_metacast(_clname);
}

int my_serialport::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void my_serialport::SR_ui_refresh_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void my_serialport::sensor_data_ui_refresh_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void my_serialport::AQ_error_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void my_serialport::AQ_error_Right_Limit_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void my_serialport::AQ_error_Left_Limit_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void my_serialport::serial_ok_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
