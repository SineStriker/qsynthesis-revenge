#ifndef OBJECTPOOL_P_H
#define OBJECTPOOL_P_H

#include "ObjectPool.h"

#include "Collections/QMChronSet.h"

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

        void objectAdded(const QString &id, QObject *obj);
        void aboutToRemoveObject(const QString &id, QObject *obj);

        void _q_objectDestroyed();
    };


}

#endif // OBJECTPOOL_P_H
