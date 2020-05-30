/****************************************************************************
** Meta object code from reading C++ file 'controlthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../AQ001-1/controlthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_controlthread_t {
    QByteArrayData data[16];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_controlthread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_controlthread_t qt_meta_stringdata_controlthread = {
    {
QT_MOC_LITERAL(0, 0, 13), // "controlthread"
QT_MOC_LITERAL(1, 14, 19), // "control_stop_signal"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 19), // "noimage_aotu_sample"
QT_MOC_LITERAL(4, 55, 4), // "path"
QT_MOC_LITERAL(5, 60, 18), // "drop_weight_signal"
QT_MOC_LITERAL(6, 79, 15), // "stopImmediately"
QT_MOC_LITERAL(7, 95, 15), // "ZV_tpye_recieve"
QT_MOC_LITERAL(8, 111, 7), // "zv_type"
QT_MOC_LITERAL(9, 119, 11), // "set_delat_T"
QT_MOC_LITERAL(10, 131, 1), // "T"
QT_MOC_LITERAL(11, 133, 12), // "set_plantype"
QT_MOC_LITERAL(12, 146, 8), // "plantype"
QT_MOC_LITERAL(13, 155, 23), // "set_noimage_sample_flag"
QT_MOC_LITERAL(14, 179, 14), // "drop_flag_slot"
QT_MOC_LITERAL(15, 194, 4) // "flag"

    },
    "controlthread\0control_stop_signal\0\0"
    "noimage_aotu_sample\0path\0drop_weight_signal\0"
    "stopImmediately\0ZV_tpye_recieve\0zv_type\0"
    "set_delat_T\0T\0set_plantype\0plantype\0"
    "set_noimage_sample_flag\0drop_flag_slot\0"
    "flag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_controlthread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    1,   60,    2, 0x06 /* Public */,
       5,    0,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   64,    2, 0x0a /* Public */,
       7,    1,   65,    2, 0x0a /* Public */,
       9,    1,   68,    2, 0x0a /* Public */,
      11,    1,   71,    2, 0x0a /* Public */,
      13,    0,   74,    2, 0x0a /* Public */,
      14,    1,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::UChar,    8,
    QMetaType::Void, QMetaType::Float,   10,
    QMetaType::Void, QMetaType::UChar,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,

       0        // eod
};

void controlthread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        controlthread *_t = static_cast<controlthread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->control_stop_signal(); break;
        case 1: _t->noimage_aotu_sample((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->drop_weight_signal(); break;
        case 3: _t->stopImmediately(); break;
        case 4: _t->ZV_tpye_recieve((*reinterpret_cast< const unsigned char(*)>(_a[1]))); break;
        case 5: _t->set_delat_T((*reinterpret_cast< const float(*)>(_a[1]))); break;
        case 6: _t->set_plantype((*reinterpret_cast< const unsigned char(*)>(_a[1]))); break;
        case 7: _t->set_noimage_sample_flag(); break;
        case 8: _t->drop_flag_slot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (controlthread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&controlthread::control_stop_signal)) {
                *result = 0;
            }
        }
        {
            typedef void (controlthread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&controlthread::noimage_aotu_sample)) {
                *result = 1;
            }
        }
        {
            typedef void (controlthread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&controlthread::drop_weight_signal)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject controlthread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_controlthread.data,
      qt_meta_data_controlthread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *controlthread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *controlthread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_controlthread.stringdata0))
        return static_cast<void*>(const_cast< controlthread*>(this));
    return QThread::qt_metacast(_clname);
}

int controlthread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void controlthread::control_stop_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void controlthread::noimage_aotu_sample(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void controlthread::drop_weight_signal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
