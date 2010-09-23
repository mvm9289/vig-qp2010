/****************************************************************************
** Meta object code from reading C++ file 'myQPushButton.h'
**
** Created: Sat Mar 27 02:48:05 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "myQPushButton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myQPushButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_myQPushButton[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      33,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_myQPushButton[] = {
    "myQPushButton\0\0stateChanged(int)\0"
    "changeState()\0"
};

const QMetaObject myQPushButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_myQPushButton,
      qt_meta_data_myQPushButton, 0 }
};

const QMetaObject *myQPushButton::metaObject() const
{
    return &staticMetaObject;
}

void *myQPushButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_myQPushButton))
        return static_cast<void*>(const_cast< myQPushButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int myQPushButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: changeState(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void myQPushButton::stateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
