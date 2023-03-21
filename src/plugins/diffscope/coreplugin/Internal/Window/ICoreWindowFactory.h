#ifndef ICOREWINDOWFACTORY_H
#define ICOREWINDOWFACTORY_H

#include "CoreApi//IWindow.h"

namespace Core {

    namespace Internal {

        class IHomeWindowFactory : public IWindowFactory {
        public:
            explicit IHomeWindowFactory();
            ~IHomeWindowFactory();

            IWindow *create(QObject *parent) override;
        };

        class IProjectWindowFactory : public IWindowFactory {
        public:
            explicit IProjectWindowFactory();
            ~IProjectWindowFactory();

            IWindow *create(QObject *parent) override;
        };
    }

}



#endif // ICOREWINDOWFACTORY_H
