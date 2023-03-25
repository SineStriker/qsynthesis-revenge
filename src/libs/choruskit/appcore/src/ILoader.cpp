#include "ILoader.h"

#include <QDebug>
#include <QHash>

#include <list>

namespace Core {

    ILoader *m_instance = nullptr;

    ILoader::ILoader(QObject *parent) : ObjectPool(parent) {
        m_instance = this;

        connect(this, &ILoader::objectAdded, this, [&](const QString &id, QObject *obj) {
            qDebug().nospace() << "ILoader: object added (" << id << ", " << obj << ")"; //
        });

        connect(this, &ILoader::aboutToRemoveObject, this, [&](const QString &id, QObject *obj) {
            qDebug().nospace() << "ILoader: object to remove (" << id << ", " << obj << ")"; //
        });
    }

    ILoader::~ILoader() {
        m_instance = nullptr;
    }

    ILoader *ILoader::instance() {
        return m_instance;
    }

}
