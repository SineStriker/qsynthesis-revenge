#include "QMJsonFormatter.h"

#define BASE_VARIANT_DECLARE(Class, TypeName)                                                      \
    QMJsonFormatter::Class::Class() {}                                                              \
                                                                                                   \
    QMJsonFormatter::Class::~Class() {}                                                             \
                                                                                                   \
    QMJsonFormatter::Type QMJsonFormatter::Class::type() const { return TypeName; }

#define EXTEND_VARIANT_DECLARE(Class)                                                              \
    QMJsonFormatter::Class::Class() { ref = nullptr; }                                              \
                                                                                                   \
    QMJsonFormatter::Class::~Class() {}                                                             \
                                                                                                   \
    QMJsonFormatter::Type QMJsonFormatter::Class::type() const { return Class##Type; }

#define ALLOCATE_VARIANT_DECLARE(Class, RealClass)                                                 \
    QMJsonFormatter::Class *QMJsonFormatter::New##Class(RealClass *ref) {                            \
        Class *val = new Class();                                                                  \
        val->ref = ref;                                                                            \
        m_memory.append(val); /* Save allocated */                                                 \
        return val;                                                                                \
    }


BASE_VARIANT_DECLARE(Variant, NoType)

EXTEND_VARIANT_DECLARE(Bool)
EXTEND_VARIANT_DECLARE(Double)
EXTEND_VARIANT_DECLARE(Int)
EXTEND_VARIANT_DECLARE(String)
EXTEND_VARIANT_DECLARE(Array)
EXTEND_VARIANT_DECLARE(Object)

EXTEND_VARIANT_DECLARE(BoolList)
EXTEND_VARIANT_DECLARE(DoubleList)
EXTEND_VARIANT_DECLARE(StringList)
EXTEND_VARIANT_DECLARE(ArrayList)
EXTEND_VARIANT_DECLARE(ObjectList)

EXTEND_VARIANT_DECLARE(BoolMap)
EXTEND_VARIANT_DECLARE(DoubleMap)
EXTEND_VARIANT_DECLARE(StringMap)
EXTEND_VARIANT_DECLARE(ArrayMap)
EXTEND_VARIANT_DECLARE(ObjectMap)

BASE_VARIANT_DECLARE(ArrayTemp, ArrayTempType)
BASE_VARIANT_DECLARE(ObjectTemp, ObjectTempType)

QMJsonFormatter::QMJsonFormatter() {
}

QMJsonFormatter::~QMJsonFormatter() {
    releaseMemory();
}

void QMJsonFormatter::releaseMemory() {
    for (auto it = m_memory.begin(); it != m_memory.end(); ++it) {
        delete *it;
    }
    m_memory.clear();
}

ALLOCATE_VARIANT_DECLARE(Bool, bool)
ALLOCATE_VARIANT_DECLARE(Double, double)
ALLOCATE_VARIANT_DECLARE(Int, int)
ALLOCATE_VARIANT_DECLARE(String, QString)
ALLOCATE_VARIANT_DECLARE(Array, QJsonArray)
ALLOCATE_VARIANT_DECLARE(Object, QJsonObject)

ALLOCATE_VARIANT_DECLARE(BoolList, QList<bool>)
ALLOCATE_VARIANT_DECLARE(DoubleList, QList<double>)
ALLOCATE_VARIANT_DECLARE(StringList, QStringList)
ALLOCATE_VARIANT_DECLARE(ArrayList, QList<QJsonArray>)
ALLOCATE_VARIANT_DECLARE(ObjectList, QList<QJsonObject>)

#define TO_STR_MAP(Class) QMap<QString, Class>

ALLOCATE_VARIANT_DECLARE(BoolMap, TO_STR_MAP(bool))
ALLOCATE_VARIANT_DECLARE(DoubleMap, TO_STR_MAP(double))
ALLOCATE_VARIANT_DECLARE(StringMap, TO_STR_MAP(QString))
ALLOCATE_VARIANT_DECLARE(ArrayMap, TO_STR_MAP(QJsonArray))
ALLOCATE_VARIANT_DECLARE(ObjectMap, TO_STR_MAP(QJsonObject))

#undef TO_STR_MAP

QMJsonFormatter::ArrayTemp *
    QMJsonFormatter::NewArrayTemp(const std::initializer_list<QMJsonFormatter::Variant *> &list) {
    ArrayTemp *val = new ArrayTemp();
    for (auto it = list.begin(); it != list.end(); ++it) {
        val->childs.append(*it);
    }
    m_memory.append(val); // Save allocated
    return val;
}

QMJsonFormatter::ObjectTemp *
    QMJsonFormatter::NewObjectTemp(const std::initializer_list<QPair<QString, Variant *>> &list) {
    ObjectTemp *val = new ObjectTemp();
    for (auto it = list.begin(); it != list.end(); ++it) {
        val->childs.insert(it->first, it->second);
    }
    m_memory.append(val); // Save allocated
    return val;
}

bool QMJsonFormatter::parse(const QJsonValue &json, QMJsonFormatter::Variant *variant) const {
    bool res = true;
    switch (variant->type()) {
        case NoType:
        case BoolType:
        case DoubleType:
        case IntType:
        case StringType:
        case ArrayType:
        case ObjectType:
        case BoolListType:
        case DoubleListType:
        case StringListType:
        case ArrayListType:
        case ObjectListType:
        case BoolMapType:
        case DoubleMapType:
        case StringMapType:
        case ArrayMapType:
        case ObjectMapType:
            res = fill(json, variant);
            break;
        case ArrayTempType: {
            if (json.isArray()) {
                QJsonArray arr = json.toArray();
                ArrayTemp *val = static_cast<ArrayTemp *>(variant);
                const auto &childs = val->childs;
                auto it0 = arr.begin();
                auto it1 = childs.begin();
                for (; it0 != arr.end() && it1 != childs.end(); ++it0, ++it1) {
                    QJsonValueRef src = *it0;
                    Variant *dest = *it1;
                    res = fill(src, dest);
                }
            } else {
                res = false;
            }
            break;
        }
        case ObjectTempType: {
            if (json.isObject()) {
                QJsonObject obj = json.toObject();
                ObjectTemp *val = static_cast<ObjectTemp *>(variant);
                const auto &childs = val->childs;
                for (auto it = childs.begin(); it != childs.end(); ++it) {
                    auto it2 = obj.find(it.key());
                    if (it2 != obj.end()) {
                        QJsonValueRef src = it2.value();
                        Variant *dest = it.value();

                        res = fill(src, dest);
                    }
                }
            } else {
                res = false;
            }
            break;
        }
        default:
            res = false;
            break;
    }
    return res;
}

bool QMJsonFormatter::fill(const QJsonValue &src, QMJsonFormatter::Variant *dest) const {

#define PARSE_LIST(Class, ListClass)                                                               \
    Class##List *dest2 = static_cast<Class##List *>(dest);                                         \
    if (dest2->ref) {                                                                              \
        QJsonArray arr = src.toArray();                                                            \
        ListClass list;                                                                            \
        bool success = true;                                                                       \
        for (auto it = arr.begin(); it != arr.end(); ++it) {                                       \
            QJsonValueRef v = *it;                                                                 \
            if (v.is##Class()) {                                                                   \
                list.append(v.to##Class());                                                        \
            } else {                                                                               \
                qDebug() << QString("QJsonFormattor: list[%1] supposed to be %2")                  \
                                .arg(QString::number(it - arr.begin()), #Class);                   \
                success = false;                                                                   \
                break;                                                                             \
            }                                                                                      \
        }                                                                                          \
        if (success) {                                                                             \
            *dest2->ref = list;                                                                    \
            res = true;                                                                            \
        }                                                                                          \
    }

#define PARSE_MAP(Class, ValueClass)                                                               \
    Class##Map *dest2 = static_cast<Class##Map *>(dest);                                           \
    if (dest2->ref) {                                                                              \
        QJsonObject obj = src.toObject();                                                          \
        QMap<QString, ValueClass> map;                                                             \
        bool success = true;                                                                       \
        for (auto it = obj.begin(); it != obj.end(); ++it) {                                       \
            QJsonValueRef v = it.value();                                                          \
            if (v.is##Class()) {                                                                   \
                map.insert(it.key(), v.to##Class());                                               \
            } else {                                                                               \
                qDebug()                                                                           \
                    << QString("QJsonFormattor: map[%1] supposed to be %2").arg(it.key(), #Class); \
                success = false;                                                                   \
                break;                                                                             \
            }                                                                                      \
        }                                                                                          \
        if (success) {                                                                             \
            *dest2->ref = map;                                                                     \
            res = true;                                                                            \
        }                                                                                          \
    }

#define PARSE_SINGLE(Class)                                                                        \
    Class *dest2;                                                                                  \
    if (dest->type() == Class##Type && (dest2 = static_cast<Class *>(dest))->ref) {                \
        *dest2->ref = src.to##Class();                                                             \
        res = true;                                                                                \
    }

    bool res = false;
    switch (src.type()) {
        case QJsonValue::Bool: {
            PARSE_SINGLE(Bool)
            break;
        }
        case QJsonValue::Double: {
            if (dest->type() == DoubleType) {
                PARSE_SINGLE(Double)
            } else {
                PARSE_SINGLE(Int)
            }
            break;
        }
        case QJsonValue::String: {
            PARSE_SINGLE(String)
            break;
        }
        case QJsonValue::Array: {
            if (dest->type() == ArrayType) {
                Array *dest2 = static_cast<Array *>(dest);
                if (dest2->ref) {
                    *dest2->ref = src.toArray();
                    res = true;
                }
            } else if (dest->type() == BoolListType) {
                PARSE_LIST(Bool, QList<bool>)
            } else if (dest->type() == DoubleListType) {
                PARSE_LIST(Double, QList<double>)
            } else if (dest->type() == StringListType) {
                PARSE_LIST(String, QStringList)
            } else if (dest->type() == StringListType) {
                PARSE_LIST(Array, QList<QJsonArray>)
            } else if (dest->type() == ObjectListType) {
                PARSE_LIST(Object, QList<QJsonObject>)
            } else if (dest->type() == ArrayTempType) {
                ArrayTemp *dest2 = static_cast<ArrayTemp *>(dest);
                res |= parse(src.toArray(), dest2); // Recursive
            }
            break;
        }
        case QJsonValue::Object: {
            if (dest->type() == ObjectType) {
                Object *dest2 = static_cast<Object *>(dest);
                if (dest2->ref) {
                    *dest2->ref = src.toObject();
                    res = true;
                }
            } else if (dest->type() == BoolMapType) {
                PARSE_MAP(Bool, bool)
            } else if (dest->type() == DoubleMapType) {
                PARSE_MAP(Double, double)
            } else if (dest->type() == StringMapType) {
                PARSE_MAP(String, QString)
            } else if (dest->type() == ArrayMapType) {
                PARSE_MAP(Array, QJsonArray)
            } else if (dest->type() == ObjectMapType) {
                PARSE_MAP(Object, QJsonObject)
            } else if (dest->type() == ObjectTempType) {
                ObjectTemp *dest2 = static_cast<ObjectTemp *>(dest);
                res |= parse(src.toObject(), dest2); // Recursive
            }
            break;
        }
        default:
            break;
    }

#undef PARSE_LIST
#undef PARSE_MAP
#undef PARSE_SINGLE

    return res;
}

#define MAP_TO_OBJECT_DECLARE(Class)                                                               \
    QJsonObject QMJsonFormatter::mapToObject(const QMap<QString, Class> &map) {                     \
        QJsonObject obj;                                                                           \
        for (auto it = map.begin(); it != map.end(); ++it) {                                       \
            obj.insert(it.key(), it.value());                                                      \
        }                                                                                          \
        return obj;                                                                                \
    }

MAP_TO_OBJECT_DECLARE(bool)
MAP_TO_OBJECT_DECLARE(double)
MAP_TO_OBJECT_DECLARE(QString)
MAP_TO_OBJECT_DECLARE(QJsonArray)
MAP_TO_OBJECT_DECLARE(QJsonObject)

#define ARRAY_TO_LIST_DECLARE(Class, RealClass, ListClass)                                         \
    ListClass QMJsonFormatter::arrayTo##Class##List(const QJsonArray &arr) {                        \
        ListClass list;                                                                            \
        for (auto it = arr.begin(); it != arr.end(); ++it) {                                       \
            if (it->is##Class()) {                                                                 \
                list.append(it->to##Class());                                                      \
            }                                                                                      \
        }                                                                                          \
        return list;                                                                               \
    }

ARRAY_TO_LIST_DECLARE(Bool, bool, QList<bool>)
ARRAY_TO_LIST_DECLARE(Double, double, QList<double>)
ARRAY_TO_LIST_DECLARE(String, QString, QStringList)
ARRAY_TO_LIST_DECLARE(Array, QJsonArray, QList<QJsonArray>)
ARRAY_TO_LIST_DECLARE(Object, QJsonObject, QList<QJsonObject>)
