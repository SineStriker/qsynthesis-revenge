#include "ILoader.h"

#include <QDebug>
#include <QHash>

#include <list>

namespace Core {

    struct ILoaderData {
        QSplashScreen *splash = nullptr;
        QString text;

        std::list<ILoader::MessageHandler> handlers;
        QHash<QString, decltype(handlers)::iterator> handlerIndexes;
    };

    Q_GLOBAL_STATIC(ILoaderData, d)

    QSplashScreen *ILoader::splash() {
        return d->splash;
    }

    void ILoader::setSplash(QSplashScreen *splash) {
        d->splash = splash;
    }

    QString ILoader::text() {
        return d->text;
    }

    void ILoader::setText(const QString &text) {
        d->text = text;
        for (auto &handler : qAsConst(d->handlers)) {
            handler.doIt(text);
        }
    }

    void ILoader::addMessageHandler(const QString &id, const MessageHandler &handler) {
        if (!handler.func) {
            qWarning() << "Core::ILoader::addMessageHandler(): trying to add null handler";
            return;
        }

        if (d->handlerIndexes.contains(id)) {
            qWarning() << "Core::ILoader::addMessageHandler(): trying to add duplicated handler:" << id;
            return;
        }

        auto it = d->handlers.insert(d->handlers.end(), handler);
        d->handlerIndexes.insert(id, it);
    }

    void ILoader::removeMessageHandler(const QString &id) {
        auto it = d->handlerIndexes.find(id);
        if (it == d->handlerIndexes.end()) {
            qWarning() << "Core::ILoader::removeMessageHandler(): handler does not exist:" << id;
            return;
        }
        d->handlers.erase(it.value());
        d->handlerIndexes.erase(it);
    }

    ILoader::MessageHandler ILoader::messageHandler(const QString &id) const {
        auto it = d->handlerIndexes.find(id);
        if (it == d->handlerIndexes.end()) {
            return {};
        }
        return *it.value();
    }

    QList<ILoader::MessageHandler> ILoader::messageHandlers() const {
        return {d->handlers.begin(), d->handlers.end()};
    }

}
