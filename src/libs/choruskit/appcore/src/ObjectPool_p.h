#ifndef OBJECTPOOL_P_H
#define OBJECTPOOL_P_H

#include "ObjectPool.h"

#include "QMChronSet.h"

namespace Core {

    class ObjectPoolPrivate : public QObject {
        Q_OBJECT
    public:
        ObjectPoolPrivate(ObjectPool *q);
        ~ObjectPoolPrivate();

        ObjectPool *q;

        // all objects
        std::list<QObject *> objects;

        // id -> objects with same id
        QHash<QString, QMChronSet<QObject *>> objectMap;

        struct Index {
            QString id;
            decltype(objects)::iterator it;
        };

        // object -> index
        QHash<QObject *, Index> objectIndexes;

        mutable QReadWriteLock objectListLock;

        QHash<QString, QVariant> globalAttributeMap;

        mutable QReadWriteLock globalAttributeLock;

        struct Checkable {
            QString id;
            QObject *obj;
            bool reverse;
        };

        QHash<QObject *, Checkable> checkableMap1;
        QHash<QString, Checkable> checkableMap2;

        void objectAdded(const QString &id, QObject *obj);
        void aboutToRemoveObject(const QString &id, QObject *obj);

        void setGlobalAttribute_helper(const QString &id, const QVariant &var, bool checkUnchanged,
                                       bool ignoreCheckable);

        void removeCheckable_helper(const QString &id, bool disconnect);
        void removeCheckable_helper(QObject *obj, bool disconnect);

        friend class ObjectPool;

    private slots:
        void _q_objectDestroyed();
        void _q_checkableToggled(bool checked);
        void _q_checkableDestroyed();
    };


}

#endif // OBJECTPOOL_P_H
