#include "ObjectPool.h"
#include "ObjectPool_p.h"

#include <QDebug>

#define DISABLE_WARNING_OBJECTS_LEFT

namespace Core {

    ObjectPoolPrivate::ObjectPoolPrivate(ObjectPool *q) : q(q) {
        connect(q, &ObjectPool::objectAdded, [this](const QString &id, QObject *obj) { Q_UNUSED(id) });

        connect(q, &ObjectPool::aboutToRemoveObject, [this](const QString &id, QObject *obj) { Q_UNUSED(id) });
    }

    ObjectPoolPrivate::~ObjectPoolPrivate() {
    }

    void ObjectPoolPrivate::objectAdded(const QString &id, QObject *obj) {
        emit q->objectAdded(id, obj);
        connect(obj, &QObject::destroyed, this, &ObjectPoolPrivate::_q_objectDestroyed);
    }

    void ObjectPoolPrivate::aboutToRemoveObject(const QString &id, QObject *obj) {
        disconnect(obj, &QObject::destroyed, this, &ObjectPoolPrivate::_q_objectDestroyed);
        emit q->aboutToRemoveObject(id, obj);
    }

    void ObjectPoolPrivate::_q_objectDestroyed() {
        q->removeObject(sender());
    }

    ObjectPool::ObjectPool(QObject *parent) : QObject(parent), d(new ObjectPoolPrivate(this)) {
    }

    ObjectPool::~ObjectPool() {
#ifndef DISABLE_WARNING_OBJECTS_LEFT
        if (!d->objects.empty()) {
            qDebug() << "There are" << d->objects.size() << "objects left in the object pool.";

            // Intentionally split debug info here, since in case the list contains
            // already deleted object we get at least the info about the number of objects;
            qDebug() << "The following objects left in the object pool of" << this << ":"
                     << QList<QObject *>(d->objects.begin(), d->objects.end());
        }
#endif
    }

    void ObjectPool::addObject(QObject *obj) {
        addObject({}, obj);
    }

    void ObjectPool::addObject(const QString &id, QObject *obj) {
        if (!obj) {
            qWarning() << "Core::ObjectPool::addObject(): trying to add null object";
            return;
        }

        {
            QWriteLocker locker(&d->objectListLock);
            auto &set = d->objectMap[id];
            if (set.contains(obj)) {
                qWarning() << "Core::ObjectPool::addObject(): trying to add duplicated object:" << id << obj;
                return;
            }

            // Add to map
            set.append(obj);

            // Add to list
            auto it = d->objects.insert(d->objects.end(), obj);

            // Add to index
            d->objectIndexes.insert(obj, {id, it});
        }

        d->objectAdded(id, obj);
    }

    void ObjectPool::addObjects(const QString &id, const QList<QObject *> &objs) {
        for (const auto &obj : objs) {
            addObject(id, obj);
        }
    }

    void ObjectPool::removeObject(QObject *obj) {
        QString id;
        {
            QReadLocker locker(&d->objectListLock);

            auto it = d->objectIndexes.find(obj);
            if (it == d->objectIndexes.end()) {
                qWarning() << "Core::ObjectPool::removeObject(): obj does not exist:" << obj;
                return;
            }

            id = it.value().id;
        }

        d->aboutToRemoveObject(id, obj);

        {
            QWriteLocker locker(&d->objectListLock);
            {
                auto it = d->objectMap.find(id);

                // Remove from map
                auto &set = it.value();
                set.remove(obj);
                if (set.isEmpty()) {
                    d->objectMap.erase(it);
                }
            }

            {
                auto it = d->objectIndexes.find(obj);

                // Remove from list
                d->objects.erase(it->it);

                // Remove from indexes
                d->objectIndexes.erase(it);
            }
        }
    }

    void ObjectPool::removeObjects(const QString &id) {
        QList<QObject *> objs;
        {
            QReadLocker locker(&d->objectListLock);
            auto it = d->objectMap.find(id);
            if (it == d->objectMap.end()) {
                return;
            }
            objs = it->values();
        }

        for (const auto &obj : qAsConst(objs)) {
            d->aboutToRemoveObject(id, obj);
        }

        {
            QWriteLocker locker(&d->objectListLock);
            auto it = d->objectMap.find(id);
            for (const auto &item : qAsConst(it.value())) {
                d->objectIndexes.remove(item);
            }
            d->objectMap.erase(it);
        }
    }

    QList<QObject *> ObjectPool::allObjects() const {
        return d->objectIndexes.keys();
    }

    QReadWriteLock *ObjectPool::listLock() const {
        return &d->objectListLock;
    }

    QList<QObject *> ObjectPool::getObjects(const QString &id) const {
        QReadLocker locker(&d->objectListLock);
        auto it2 = d->objectMap.find(id);
        if (it2 != d->objectMap.end()) {
            return it2->values();
        }
        return {};
    }

    QObject *ObjectPool::getFirstObject(const QString &id) const {
        QReadLocker locker(&d->objectListLock);
        auto it2 = d->objectMap.find(id);
        if (it2 != d->objectMap.end()) {
            return it2->isEmpty() ? nullptr : *it2->begin();
        }
        return nullptr;
    }

}