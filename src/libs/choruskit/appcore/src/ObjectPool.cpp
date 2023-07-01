#include "ObjectPool.h"
#include "ObjectPool_p.h"

#include <QDebug>
#include <QMetaMethod>

#define DISABLE_WARNING_OBJECTS_LEFT

namespace Core {

#define myWarning(func) (qWarning().nospace() << "Core::ObjectPool::" << (func) << "():").space()

    ObjectPoolPrivate::ObjectPoolPrivate(ObjectPool *q) : q(q) {
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

    void ObjectPoolPrivate::setGlobalAttribute_helper(const QString &id, const QVariant &var, bool checkUnchanged,
                                                      bool ignoreCheckable) {
        QVariant org;
        {
            QWriteLocker locker(&globalAttributeLock);

            auto it = globalAttributeMap.find(id);
            if (it != globalAttributeMap.end()) {
                org = it.value();
                if (var.isNull() || !var.isValid()) {
                    globalAttributeMap.erase(it);
                } else {
                    it.value() = var;
                }
            } else {
                if (var.isNull() || !var.isValid()) {
                    // ...
                } else {
                    globalAttributeMap.insert(id, var);
                }
            }

            if (checkUnchanged && var == org) {
                return;
            }
        }

        if (!ignoreCheckable) {
            QReadLocker locker(&globalAttributeLock);
            // Find possible checkable
            auto it2 = checkableMap2.find(id);
            if (it2 != checkableMap2.end()) {
                const auto &checkable = it2.value();
                locker.unlock();

                // call setChecked
                QMetaObject::invokeMethod(checkable.obj, "setChecked", Qt::AutoConnection,
                                          Q_ARG(bool, checkable.reverse ^ var.toBool()));
            }
        }
        emit q->globalAttributeChanged(id, var, org);
    }

    void ObjectPoolPrivate::removeCheckable_helper(const QString &id, bool disconnect) {
        QWriteLocker locker(&globalAttributeLock);

        auto it = checkableMap2.find(id);
        if (it == checkableMap2.end()) {
            return;
        }
        auto obj = it->obj;

        checkableMap1.remove(obj);
        checkableMap2.erase(it);

        if (!disconnect) {
            return;
        }

        QObject::disconnect(obj, SIGNAL(toggled(bool)), this, SLOT(_q_checkableToggled(bool)));
        QObject::disconnect(obj, &QObject::destroyed, this, &ObjectPoolPrivate::_q_checkableDestroyed);
    }

    void ObjectPoolPrivate::removeCheckable_helper(QObject *obj, bool disconnect) {
        QWriteLocker locker(&globalAttributeLock);

        auto it = checkableMap1.find(obj);
        if (it == checkableMap1.end()) {
            return;
        }
        auto id = it->id;
        auto reverse = it->reverse;

        checkableMap1.erase(it);
        checkableMap2.remove(id);

        if (!disconnect) {
            return;
        }

        QObject::disconnect(obj, SIGNAL(toggled(bool)), this, SLOT(_q_checkableToggled(bool)));
        QObject::disconnect(obj, &QObject::destroyed, this, &ObjectPoolPrivate::_q_checkableDestroyed);
    }

    void ObjectPoolPrivate::_q_objectDestroyed() {
        q->removeObject(sender());
    }

    void ObjectPoolPrivate::_q_checkableToggled(bool checked) {
        Checkable checkable;
        {
            QReadLocker locker(&globalAttributeLock);
            checkable = checkableMap1.value(sender(), {});
            if (checkable.id.isEmpty()) {
                return;
            }
        }
        setGlobalAttribute_helper(checkable.id, checkable.reverse ^ checked, true, true);
    }

    void ObjectPoolPrivate::_q_checkableDestroyed() {
        removeCheckable_helper(sender(), false);
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
        delete d;
    }

    void ObjectPool::addObject(QObject *obj) {
        addObject({}, obj);
    }

    void ObjectPool::addObject(const QString &id, QObject *obj) {
        if (!obj) {
            myWarning(__func__) << "trying to add null object";
            return;
        }

        {
            QWriteLocker locker(&d->objectListLock);
            auto &set = d->objectMap[id];
            if (!set.append(obj).second) {
                myWarning(__func__) << "trying to add duplicated object:" << id << obj;
                return;
            }

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
                myWarning(__func__) << "obj does not exist:" << obj;
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

    QVariant ObjectPool::globalAttribute(const QString &id) const {
        QReadLocker locker(&d->globalAttributeLock);
        return d->globalAttributeMap.value(id, {});
    }

    void ObjectPool::setGlobalAttribute(const QString &id, const QVariant &var, bool checkUnchanged) {
        d->setGlobalAttribute_helper(id, var, checkUnchanged, false);
    }

    void ObjectPool::addCheckable(const QString &id, QObject *obj, bool reverse) {
        {
            QReadLocker locker(&d->globalAttributeLock);
            if (d->checkableMap2.contains(id)) {
                return;
            }
        }
        {
            QWriteLocker locker(&d->globalAttributeLock);

            d->checkableMap1.insert(obj, {id, obj, reverse});
            d->checkableMap2.insert(id, {id, obj, reverse});

            connect(obj, SIGNAL(toggled(bool)), d, SLOT(_q_checkableToggled(bool)));
            connect(obj, &QObject::destroyed, d, &ObjectPoolPrivate::_q_checkableDestroyed);
        }

        d->setGlobalAttribute_helper(id, reverse ^ obj->property("checked").toBool(), true, true);
    }

    void ObjectPool::removeCheckable(const QString &id) {
        d->removeCheckable_helper(id, true);
    }

    void ObjectPool::removeCheckable(QObject *obj) {
        d->removeCheckable_helper(obj, true);
    }

    void ObjectPool::requestGlobalEvent(const QString &id, const QVariantHash &args) {
        emit globalEventRequested(id, args);
    }

}