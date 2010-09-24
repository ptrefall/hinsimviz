/****************************************************************************
** Meta object code from reading C++ file 'IWrapper.h'
**
** Created: Fri 24. Sep 12:57:09 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtExtensions/IWrapper.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IWrapper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IWrapper[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   10,    9,    9, 0x0a,
      44,   38,    9,    9, 0x0a,
      70,   38,    9,    9, 0x0a,
      96,   38,    9,    9, 0x0a,
     119,   38,    9,    9, 0x0a,
     147,  142,    9,    9, 0x0a,
     172,   38,    9,    9, 0x0a,
     198,   38,    9,    9, 0x0a,
     224,   38,    9,    9, 0x0a,
     250,   38,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_IWrapper[] = {
    "IWrapper\0\0state\0slotStateChanged(int)\0"
    "value\0slotDValueChanged(double)\0"
    "slotFValueChanged(double)\0"
    "slotIValueChanged(int)\0slotUValueChanged(int)\0"
    "text\0slotTextChanged(QString)\0"
    "slot1ValueChanged(double)\0"
    "slot2ValueChanged(double)\0"
    "slot3ValueChanged(double)\0"
    "slot4ValueChanged(double)\0"
};

const QMetaObject IWrapper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IWrapper,
      qt_meta_data_IWrapper, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IWrapper::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IWrapper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IWrapper::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IWrapper))
        return static_cast<void*>(const_cast< IWrapper*>(this));
    return QObject::qt_metacast(_clname);
}

int IWrapper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slotDValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: slotFValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: slotIValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: slotUValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: slotTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: slot1ValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: slot2ValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: slot3ValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: slot4ValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
