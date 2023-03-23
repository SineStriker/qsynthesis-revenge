#ifndef ILOADER_H
#define ILOADER_H

#include <QSplashScreen>

#include <functional>

#include "CkAppCoreGlobal.h"

class QSplashScreen;

namespace Core {

    class CKAPPCORE_API ILoader {
    public:
        static QSplashScreen *splash();
        static void setSplash(QSplashScreen *splash);

        static QString text();
        static void setText(const QString &text);

    public:
        typedef void (*MessageHandlerFunc)(void *, const QString &);

        struct MessageHandler {
            void *data;
            MessageHandlerFunc func;

            inline MessageHandler() : MessageHandler(nullptr, nullptr) {
            }

            inline MessageHandler(void *data, MessageHandlerFunc func) : data(data), func(func) {
            }

            inline void doIt(const QString &text) const {
                func ? func(data, text) : void();
            }
        };

        static void addMessageHandler(const QString &id, const MessageHandler &handler);
        static void removeMessageHandler(const QString &id);
        MessageHandler messageHandler(const QString &id) const;
        QList<MessageHandler> messageHandlers() const;
    };

}


#endif // ILOADER_H
