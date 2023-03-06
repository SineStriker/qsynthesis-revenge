#include "ImportManager.h"

#include <QApplication>

namespace Core {

    namespace Internal {

        ImportManager::ImportManager() {
        }

        ImportManager::~ImportManager() {
        }

        bool ImportManager::initialize(const QStringList &arguments, QString *errorMessage) {
            qDebug() << "initialize ImportManager Plugin";
            return true;
        }

        void ImportManager::extensionsInitialized() {
            qDebug() << "extensions initialized ImportManager Plugin";
        }

        bool ImportManager::delayedInitialize() {
            qDebug() << "delayed initialize ImportManager Plugin";
            return true;
        }

    }

}