#ifndef ILOADER_H
#define ILOADER_H

#include <QSplashScreen>

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
        using MessageHandler = void (*)(intptr_t data, const QString &);

        static void addMessageHandler(MessageHandler handler, intptr_t data);
        static void removeMessageHandler(MessageHandler handler, intptr_t data);
    };

}


#endif // ILOADER_H
