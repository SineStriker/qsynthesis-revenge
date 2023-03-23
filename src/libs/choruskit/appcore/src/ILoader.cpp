#include "ILoader.h"

#include <QDebug>
#include <QHash>

#include <list>

namespace Core {

    struct ILoaderData {
        QSplashScreen *splash = nullptr;
        QString text;

        struct Handler {
            ILoader::MessageHandler handler;
            intptr_t data;
        };

        std::list<Handler> handlers;
        QHash<intptr_t, decltype(handlers)::iterator> handlerIndexes;
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
        for (auto item : qAsConst(d->handlers)) {
            item.handler(item.data, text);
        }
    }

    void ILoader::addMessageHandler(ILoader::MessageHandler handler, intptr_t data) {
        auto handler_ptr = intptr_t(handler);
        if (!handler_ptr) {
            qWarning() << "Core::ILoader::addMessageHandler(): trying to add null handler";
            return;
        }
        auto it = d->handlers.insert(d->handlers.end(), {handler, data});
        d->handlerIndexes.insert(handler_ptr, it);
    }

    void ILoader::removeMessageHandler(ILoader::MessageHandler handler, intptr_t data) {
        auto handler_ptr = intptr_t(handler);
        if (!handler_ptr) {
            qWarning() << "Core::ILoader::removeMessageHandler(): trying to remove null handler";
            return;
        }

        auto it = d->handlerIndexes.find(handler_ptr);
        if (it == d->handlerIndexes.end()) {
            qWarning() << "Core::ILoader::removeMessageHandler(): handler does not exist:" << handler_ptr;
            return;
        }
        d->handlers.erase(it.value());
        d->handlerIndexes.erase(it);
    }

}
