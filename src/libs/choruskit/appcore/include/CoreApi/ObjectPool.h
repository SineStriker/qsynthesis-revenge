#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <QReadWriteLock>
#include <QVariant>
#include <QWidget>

#include "CkAppCoreGlobal.h"

namespace Core {

    class ObjectPoolPrivate;

    class CKAPPCORE_API ObjectPool : public QObject {
        Q_OBJECT
    public:
        explicit ObjectPool(QObject *parent = nullptr);
        ~ObjectPool();

    public:
        void addObject(QObject *obj);
        void addObject(const QString &id, QObject *obj);
        void addObjects(const QString &id, const QList<QObject *> &objs);
        void removeObject(QObject *obj);
        void removeObjects(const QString &id);
        QList<QObject *> allObjects() const;
        QReadWriteLock *listLock() const;

        QList<QObject *> getObjects(const QString &id) const;

        template <typename T, typename Predicate>
        QList<T *> getObjects(Predicate predicate) {
            QReadLocker lock(listLock());
            QList<T *> results;
            QList<QObject *> all = allObjects();
            foreach (QObject *obj, all) {
                T *result = qobject_cast<T *>(obj);
                if (result && predicate(result))
                    results += result;
            }
            return results;
        }

        template <class T>
        QList<T *> getObjects() const {
            QReadLocker lock(listLock());
            QList<QObject *> all = allObjects();
            QList<QObject *> res;
            foreach (QObject *obj, all) {
                if (T *result = qobject_cast<T *>(obj))
                    res.append(result);
            }
            return res;
        }

        QObject *getFirstObject(const QString &id) const;

        template <typename T>
        T *getFirstObject() {
            QReadLocker lock(listLock());
            QList<QObject *> all = allObjects();
            foreach (QObject *obj, all) {
                if (T *result = qobject_cast<T *>(obj))
                    return result;
            }
            return 0;
        }

        template <typename T, typename Predicate>
        T *getFirstObject(Predicate predicate) {
            QReadLocker lock(listLock());
            QList<QObject *> all = allObjects();
            foreach (QObject *obj, all) {
                if (T *result = qobject_cast<T *>(obj))
                    if (predicate(result))
                        return result;
            }
            return 0;
        }

        QVariant globalAttribute(const QString &id) const;
        void setGlobalAttribute(const QString &id, const QVariant &var, bool checkUnchanged = true);

        void addCheckable(const QString &id, QObject *obj, bool reverse = false);
        void removeCheckable(const QString &id);
        void removeCheckable(QObject *obj);

        void requestGlobalEvent(const QString &id, const QVariantHash &args = {});

    signals:
        void objectAdded(const QString &id, QObject *obj);
        void aboutToRemoveObject(const QString &id, QObject *obj);

        void globalAttributeChanged(const QString &id, const QVariant &var, const QVariant &orgVar = {});
        void globalEventRequested(const QString &id, const QVariantHash &args);

    private:
        ObjectPoolPrivate *d;

        friend class ObjectPoolPrivate;
    };

}


#endif // OBJECTPOOL_H
