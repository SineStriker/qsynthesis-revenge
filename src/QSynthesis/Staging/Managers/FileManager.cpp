#include "FileManager.h"
#include "private/FileManager_p.h"

Q_SINGLETON_DECLARE(FileManager)

FileManager::FileManager(QObject *parent) : FileManager(*new FileManagerPrivate(), parent) {
}

FileManager::~FileManager() {
}

FileManager::FileManager(FileManagerPrivate &d, QObject *parent) : BasicManager(d, parent) {
    construct();
    d.init();
}
