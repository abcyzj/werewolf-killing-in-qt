/****************************************************************************
** Meta object code from reading C++ file 'logicworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../server/logicworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logicworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LogicWorker_t {
    QByteArrayData data[8];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogicWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogicWorker_t qt_meta_stringdata_LogicWorker = {
    {
QT_MOC_LITERAL(0, 0, 11), // "LogicWorker"
QT_MOC_LITERAL(1, 12, 12), // "hasNewClient"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "listenError"
QT_MOC_LITERAL(4, 38, 8), // "gameOver"
QT_MOC_LITERAL(5, 47, 9), // "startGame"
QT_MOC_LITERAL(6, 57, 3), // "run"
QT_MOC_LITERAL(7, 61, 9) // "addClient"

    },
    "LogicWorker\0hasNewClient\0\0listenError\0"
    "gameOver\0startGame\0run\0addClient"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogicWorker[] = {

 // content:
       7,       // revision
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
       7,    0,   49,    2, 0x08 /* Private */,

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

void LogicWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LogicWorker *_t = static_cast<LogicWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hasNewClient(); break;
        case 1: _t->listenError(); break;
        case 2: _t->gameOver(); break;
        case 3: _t->startGame(); break;
        case 4: _t->run(); break;
        case 5: _t->addClient(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LogicWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LogicWorker::hasNewClient)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (LogicWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LogicWorker::listenError)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (LogicWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LogicWorker::gameOver)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject LogicWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LogicWorker.data,
      qt_meta_data_LogicWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LogicWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogicWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LogicWorker.stringdata0))
        return static_cast<void*>(const_cast< LogicWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int LogicWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void LogicWorker::hasNewClient()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void LogicWorker::listenError()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void LogicWorker::gameOver()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
