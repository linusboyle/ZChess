/****************************************************************************
** Meta object code from reading C++ file 'chessboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chessboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chessboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChessBoard_t {
    QByteArrayData data[16];
    char stringdata0[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChessBoard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChessBoard_t qt_meta_stringdata_ChessBoard = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ChessBoard"
QT_MOC_LITERAL(1, 11, 10), // "chessMoved"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "chessid"
QT_MOC_LITERAL(4, 31, 4), // "newx"
QT_MOC_LITERAL(5, 36, 4), // "newy"
QT_MOC_LITERAL(6, 41, 10), // "chessEaten"
QT_MOC_LITERAL(7, 52, 14), // "initBoardState"
QT_MOC_LITERAL(8, 67, 10), // "clearBoard"
QT_MOC_LITERAL(9, 78, 9), // "setEnable"
QT_MOC_LITERAL(10, 88, 4), // "side"
QT_MOC_LITERAL(11, 93, 9), // "moveChess"
QT_MOC_LITERAL(12, 103, 2), // "id"
QT_MOC_LITERAL(13, 106, 1), // "x"
QT_MOC_LITERAL(14, 108, 1), // "y"
QT_MOC_LITERAL(15, 110, 13) // "chessSelected"

    },
    "ChessBoard\0chessMoved\0\0chessid\0newx\0"
    "newy\0chessEaten\0initBoardState\0"
    "clearBoard\0setEnable\0side\0moveChess\0"
    "id\0x\0y\0chessSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChessBoard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x06 /* Public */,
       6,    1,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    1,   61,    2, 0x0a /* Public */,
      11,    3,   64,    2, 0x0a /* Public */,
      15,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   12,   13,   14,
    QMetaType::Void,

       0        // eod
};

void ChessBoard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChessBoard *_t = static_cast<ChessBoard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->chessMoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->chessEaten((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->initBoardState(); break;
        case 3: _t->clearBoard(); break;
        case 4: _t->setEnable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->moveChess((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->chessSelected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChessBoard::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChessBoard::chessMoved)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChessBoard::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChessBoard::chessEaten)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChessBoard::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_ChessBoard.data,
      qt_meta_data_ChessBoard,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ChessBoard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChessBoard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChessBoard.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int ChessBoard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
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
void ChessBoard::chessMoved(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChessBoard::chessEaten(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
