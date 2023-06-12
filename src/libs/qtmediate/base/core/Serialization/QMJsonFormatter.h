#ifndef QMJSONFORMATTER_H
#define QMJSONFORMATTER_H

/*
 * Copyright 2019-2020 by Sinestr.
 * All rights reserved. Distributed under the terms of the MIT license.
 * This util helps to read json value to serialized data structure.
 */

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include "QMGlobal.h"

class QMCORE_EXPORT QMJsonFormatter {
private:
    enum Type {
        NoType,
        // Single
        BoolType,
        DoubleType,
        IntType,
        StringType,
        ArrayType,
        ObjectType,
        // List
        BoolListType,
        StringListType,
        DoubleListType,
        ArrayListType,
        ObjectListType,
        // Map
        BoolMapType,
        DoubleMapType,
        StringMapType,
        ArrayMapType,
        ObjectMapType,
        // Recursive
        ArrayTempType,
        ObjectTempType
    };

    class Variant {
    public:
        Variant();
        virtual ~Variant();
        virtual Type type() const;
    };

    class Bool : public Variant {
    public:
        Bool();
        ~Bool() override;
        Type type() const override;

        bool *ref;
    };

    class Double : public Variant {
    public:
        Double();
        ~Double() override;
        Type type() const override;

        double *ref;
    };

    class Int : public Variant {
    public:
        Int();
        ~Int() override;
        Type type() const override;

        int *ref;
    };

    class String : public Variant {
    public:
        String();
        ~String() override;
        Type type() const override;

        QString *ref;
    };

    class Array : public Variant {
    public:
        Array();
        ~Array() override;
        Type type() const override;

        QJsonArray *ref;
    };

    class Object : public Variant {
    public:
        Object();
        ~Object() override;
        Type type() const override;

        QJsonObject *ref;
    };

    class BoolList : public Variant {
    public:
        BoolList();
        ~BoolList() override;
        Type type() const override;

        QList<bool> *ref;
    };

    class DoubleList : public Variant {
    public:
        DoubleList();
        ~DoubleList() override;
        Type type() const override;

        QList<double> *ref;
    };

    class StringList : public Variant {
    public:
        StringList();
        ~StringList() override;
        Type type() const override;

        QStringList *ref;
    };

    class ArrayList : public Variant {
    public:
        ArrayList();
        ~ArrayList() override;
        Type type() const override;

        QList<QJsonArray> *ref;
    };

    class ObjectList : public Variant {
    public:
        ObjectList();
        ~ObjectList() override;
        Type type() const override;

        QList<QJsonObject> *ref;
    };

    class BoolMap : public Variant {
    public:
        BoolMap();
        ~BoolMap() override;
        Type type() const override;

        QMap<QString, bool> *ref;
    };

    class DoubleMap : public Variant {
    public:
        DoubleMap();
        ~DoubleMap() override;
        Type type() const override;

        QMap<QString, double> *ref;
    };

    class StringMap : public Variant {
    public:
        StringMap();
        ~StringMap() override;
        Type type() const override;

        QMap<QString, QString> *ref;
    };

    class ArrayMap : public Variant {
    public:
        ArrayMap();
        ~ArrayMap() override;
        Type type() const override;

        QMap<QString, QJsonArray> *ref;
    };

    class ObjectMap : public Variant {
    public:
        ObjectMap();
        ~ObjectMap() override;
        Type type() const override;

        QMap<QString, QJsonObject> *ref;
    };

    class ArrayTemp : public Variant {
    public:
        ArrayTemp();
        ~ArrayTemp() override;
        Type type() const override;

        QList<Variant *> childs;
    };

    class ObjectTemp : public Variant {
    public:
        ObjectTemp();
        ~ObjectTemp() override;
        Type type() const override;

        QMap<QString, Variant *> childs;
    };

public:
    QMJsonFormatter();
    ~QMJsonFormatter();

    void releaseMemory();

public:
    Bool *NewBool(bool *ref);
    Double *NewDouble(double *ref);
    Int *NewInt(int *ref);
    String *NewString(QString *ref);
    Array *NewArray(QJsonArray *ref);
    Object *NewObject(QJsonObject *ref);

    BoolList *NewBoolList(QList<bool> *ref);
    DoubleList *NewDoubleList(QList<double> *ref);
    StringList *NewStringList(QStringList *ref);
    ArrayList *NewArrayList(QList<QJsonArray> *ref);
    ObjectList *NewObjectList(QList<QJsonObject> *ref);

    BoolMap *NewBoolMap(QMap<QString, bool> *ref);
    DoubleMap *NewDoubleMap(QMap<QString, double> *ref);
    StringMap *NewStringMap(QMap<QString, QString> *ref);
    ArrayMap *NewArrayMap(QMap<QString, QJsonArray> *ref);
    ObjectMap *NewObjectMap(QMap<QString, QJsonObject> *ref);

    ArrayTemp *NewArrayTemp(const std::initializer_list<Variant *> &list);
    ObjectTemp *NewObjectTemp(const std::initializer_list<QPair<QString, Variant *>> &list);

    inline Bool *B(bool *ref) {
        return NewBool(ref);
    }
    inline Double *D(double *ref) {
        return NewDouble(ref);
    }
    inline Int *I(int *ref) {
        return NewInt(ref);
    }
    inline String *S(QString *ref) {
        return NewString(ref);
    }
    inline Array *A(QJsonArray *ref) {
        return NewArray(ref);
    }
    inline Object *O(QJsonObject *ref) {
        return NewObject(ref);
    }
    inline BoolList *BL(QList<bool> *ref) {
        return NewBoolList(ref);
    }
    inline DoubleList *DL(QList<double> *ref) {
        return NewDoubleList(ref);
    }
    inline StringList *SL(QStringList *ref) {
        return NewStringList(ref);
    }
    inline ArrayList *AL(QList<QJsonArray> *ref) {
        return NewArrayList(ref);
    }
    inline ObjectList *OL(QList<QJsonObject> *ref) {
        return NewObjectList(ref);
    }
    inline BoolMap *BM(QMap<QString, bool> *ref) {
        return NewBoolMap(ref);
    }
    inline DoubleMap *DM(QMap<QString, double> *ref) {
        return NewDoubleMap(ref);
    }
    inline StringMap *SM(QMap<QString, QString> *ref) {
        return NewStringMap(ref);
    }
    inline ArrayMap *AM(QMap<QString, QJsonArray> *ref) {
        return NewArrayMap(ref);
    }
    inline ObjectMap *OM(QMap<QString, QJsonObject> *ref) {
        return NewObjectMap(ref);
    }

    inline ArrayTemp *AT(const std::initializer_list<Variant *> &list) {
        return NewArrayTemp(list);
    }
    inline ObjectTemp *OT(const std::initializer_list<QPair<QString, Variant *>> &list) {
        return NewObjectTemp(list);
    }

    bool parse(const QJsonValue &json, Variant *variant) const;

private:
    bool fill(const QJsonValue &src, Variant *dest) const;

    QList<Variant *> m_memory;

public:
    static QJsonObject mapToObject(const QMap<QString, bool> &map);
    static QJsonObject mapToObject(const QMap<QString, double> &map);
    static QJsonObject mapToObject(const QMap<QString, QString> &map);
    static QJsonObject mapToObject(const QMap<QString, QJsonArray> &map);
    static QJsonObject mapToObject(const QMap<QString, QJsonObject> &map);

    static QList<bool> arrayToBoolList(const QJsonArray &arr);
    static QList<double> arrayToDoubleList(const QJsonArray &arr);
    static QStringList arrayToStringList(const QJsonArray &arr);
    static QList<QJsonArray> arrayToArrayList(const QJsonArray &arr);
    static QList<QJsonObject> arrayToObjectList(const QJsonArray &arr);
};

#endif // QMJSONFORMATTER_H
