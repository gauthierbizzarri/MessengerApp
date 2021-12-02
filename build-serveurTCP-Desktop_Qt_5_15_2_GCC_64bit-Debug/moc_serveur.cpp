/****************************************************************************
** Meta object code from reading C++ file 'serveur.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../SERVER/serveur.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serveur.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_serveur_t {
    QByteArrayData data[10];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_serveur_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_serveur_t qt_meta_stringdata_serveur = {
    {
QT_MOC_LITERAL(0, 0, 7), // "serveur"
QT_MOC_LITERAL(1, 8, 13), // "nouveauJoueur"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 16), // "sockDisconnected"
QT_MOC_LITERAL(4, 40, 13), // "joueurMeParle"
QT_MOC_LITERAL(5, 54, 7), // "Proceed"
QT_MOC_LITERAL(6, 62, 4), // "json"
QT_MOC_LITERAL(7, 67, 17), // "check_credentials"
QT_MOC_LITERAL(8, 85, 5), // "jsons"
QT_MOC_LITERAL(9, 91, 12) // "send_message"

    },
    "serveur\0nouveauJoueur\0\0sockDisconnected\0"
    "joueurMeParle\0Proceed\0json\0check_credentials\0"
    "jsons\0send_message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_serveur[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    1,   47,    2, 0x08 /* Private */,
       7,    1,   50,    2, 0x08 /* Private */,
       9,    1,   53,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    6,
    QMetaType::Void, QMetaType::QJsonObject,    8,
    QMetaType::Void, QMetaType::QJsonObject,    6,

       0        // eod
};

void serveur::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<serveur *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->nouveauJoueur(); break;
        case 1: _t->sockDisconnected(); break;
        case 2: _t->joueurMeParle(); break;
        case 3: _t->Proceed((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->check_credentials((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 5: _t->send_message((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject serveur::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_serveur.data,
    qt_meta_data_serveur,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *serveur::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *serveur::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_serveur.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int serveur::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
