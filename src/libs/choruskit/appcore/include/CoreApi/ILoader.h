#ifndef ILOADER_H
#define ILOADER_H

#include <QSplashScreen>

#include "ObjectPool.h"

namespace Core {

    class CKAPPCORE_API ILoader : public ObjectPool {
        Q_OBJECT
    public:
        explicit ILoader(QObject *parent = nullptr);
        ~ILoader();

        static ILoader *instance();
    };

}


#endif // ILOADER_H
