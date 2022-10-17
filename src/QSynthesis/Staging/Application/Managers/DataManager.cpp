#include "DataManager.h"
#include "DataManager_p.h"

#include <QDebug>

Q_SINGLETON_DECLARE(DataManager)

DataManager::DataManager(QObject *parent) : DataManager(*new DataManagerPrivate(), parent) {
}

DataManager::~DataManager() {
}

bool DataManager::load() {
    return true;
}

bool DataManager::save() {
    return true;
}

void DataManager::reloadStrings() {
}

DataManager::DataManager(DataManagerPrivate &d, QObject *parent) : BasicManager(d, parent) {
    construct();

    d.init();
}
