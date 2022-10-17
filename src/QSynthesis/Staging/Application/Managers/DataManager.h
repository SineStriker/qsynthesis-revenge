#ifndef DATAMANAGER_H
#define DATAMANAGER_H

/*
 * Usage: Manage translations
 *
 *
 */

#include "qsutils_macros.h"
#include "Vendor/Objects/BasicManager.h"

#define qData DataManager::instance()

// Always call this macro at the end of constructor
#define Q_TR_NOTIFY(T)                                                                             \
    reloadStrings();                                                                               \
    connect(qData, &DataManager::stringUpdated, this, &T::reloadStrings);

class DataManagerPrivate;

class DataManager : public BasicManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DataManager)

    Q_SINGLETON(DataManager)
public:
    DataManager(QObject *parent = nullptr);
    ~DataManager();

public:
    bool load();
    bool save();

    void reloadStrings();

protected:
    DataManager(DataManagerPrivate &d, QObject *parent = nullptr);

signals:
    void stringUpdated();
};

#endif // DATAMANAGER_H
