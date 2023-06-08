#ifndef CHORUSKIT_ACETREESERIALIZER_H
#define CHORUSKIT_ACETREESERIALIZER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class AceTreeItem;

    class AceTreeSerializerPrivate;

    class QSFRAMEWORK_API AceTreeSerializer {
    public:
        explicit AceTreeSerializer(const QString &id);
        AceTreeSerializer(const QString &id, QJsonValue::Type preferredType);
        virtual ~AceTreeSerializer();

        enum Result {
            Failed,
            Success,
            Unhandled,
        };

        struct ValueRef {
            union {
                QJsonArray *a;
                QJsonObject *o;
            };
            bool is_object;

            inline ValueRef(QJsonArray *a);
            inline ValueRef(QJsonObject *o);
        };

        enum AcceptValueType {
            NoAccept = 0,
            Property = 0x1000,
            DynamicData = 0x2000,
        };

        class AcceptValueTypeStrict {
        public:
            AcceptValueTypeStrict(int i)
                : AcceptValueTypeStrict(static_cast<AcceptValueType>(0xF000 & i),
                                        static_cast<QJsonValue::Type>(0xFF & i)) {
            }
            AcceptValueTypeStrict(AcceptValueType at) : AcceptValueTypeStrict(at, {}) {
            }
            AcceptValueTypeStrict(const QJsonValue &jt) : AcceptValueTypeStrict({}, jt) {
            }
            AcceptValueTypeStrict(AcceptValueType at, const QJsonValue &jt) : at(at), jt(jt) {
            }
            AcceptValueType aType() const {
                return at;
            }
            QJsonValue::Type jType() const {
                return jt.type();
            }
            QJsonValue jValue() const {
                return jt;
            }
            bool validate(QJsonValue::Type type) const {
                return jt == QJsonValue::Null ? true : (jt.type() == type);
            }
            bool operator==(const AcceptValueTypeStrict &other) const {
                return at == other.at && jt == other.jt;
            }
            bool operator!=(const AcceptValueTypeStrict &other) const {
                return !(*this == other);
            }
            bool operator==(AcceptValueType other) const {
                return at == other;
            }
            bool operator!=(AcceptValueType other) const {
                return !(*this == other);
            }
            bool operator==(QJsonValue::Type other) const {
                return jt == other;
            }
            bool operator!=(QJsonValue::Type other) const {
                return !(*this == other);
            }

        private:
            AcceptValueType at;
            QJsonValue jt;
        };

        struct ChildOrProperty {
            union {
                const AceTreeItem *i;
                const QVariant *v;
            };
            bool is_item;
            AcceptValueType value_type;

            inline ChildOrProperty(const AceTreeItem *i);
            inline ChildOrProperty(const QVariant *v, AcceptValueType value_type = Property);
        };

        struct ArrayOptions {
            QString typeField;
            Result (*reader)(int, const QJsonValue &, AceTreeItem *, void *);
            Result (*writer)(int, QJsonArray *, const ChildOrProperty &, void *);
            void (*creator)(AceTreeItem *, void *);
        };

        struct ObjectOptions {
            AcceptValueType acceptAll;
            QHash<QString, AcceptValueTypeStrict> acceptOnes;
            Result (*reader)(const QString &, const QJsonValue &, AceTreeItem *, void *);
            Result (*writer)(const QString &, QJsonObject *, const ChildOrProperty &, void *);
            void (*creator)(AceTreeItem *, void *);
        };

        QString id() const;

        QJsonValue::Type preferredType() const;
        void setPreferredType(QJsonValue::Type type);

        void *userdata() const;
        void setUserdata(void *userdata);

    public:
        ArrayOptions arrayOptions() const;
        void setArrayOptions(const ArrayOptions &opt);
        void setArrayTypeField(const QString &key);

        ObjectOptions objectOptions() const;
        void setObjectOptions(const ObjectOptions &opt);
        void setObjectAcceptAll(AcceptValueType type = Property);
        void setObjectAcceptOnes(const QHash<QString, AcceptValueTypeStrict> &hash);

        QString name() const;

    public:
        bool addChild(const QString &key, AceTreeSerializer *child);
        bool removeChild(const QString &key);
        bool removeChild(AceTreeSerializer *child);
        bool removeChildById(const QString &id);

        AceTreeSerializer *child(const QString &id) const;
        QList<AceTreeSerializer *> children() const;

        AceTreeSerializer *parent() const;
        AceTreeSerializer *root() const;

        QStringList keys() const;
        AceTreeSerializer *keyToChild(const QString &key) const;

    public:
        virtual bool readValue(const QJsonValue &value, AceTreeItem *item);
        virtual bool readArray(const QJsonArray &array, AceTreeItem *item);
        virtual bool readObject(const QJsonObject &object, AceTreeItem *item);

        virtual bool writeValue(const ValueRef &ref, const ChildOrProperty &cop);
        virtual bool writeArray(QJsonArray *array, const AceTreeItem *item);
        virtual bool writeObject(QJsonObject *object, const AceTreeItem *item);

        virtual void createValue(AceTreeItem *item);
        virtual void createArray(AceTreeItem *item);
        virtual void createObject(AceTreeItem *item);

    private:
        AceTreeSerializerPrivate *d;

        friend class AceTreeSerializerPrivate;
    };

    AceTreeSerializer::ValueRef::ValueRef(QJsonArray *a) : a(a), is_object(false) {
    }

    AceTreeSerializer::ValueRef::ValueRef(QJsonObject *o) : o(o), is_object(true) {
    }

    AceTreeSerializer::ChildOrProperty::ChildOrProperty(const AceTreeItem *i) : i(i), is_item(true) {
    }

    AceTreeSerializer::ChildOrProperty::ChildOrProperty(const QVariant *v, AcceptValueType value_type)
        : v(v), is_item(false), value_type(value_type) {
    }

}



#endif // CHORUSKIT_ACETREESERIALIZER_H
