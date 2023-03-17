#ifndef ICORE_H
#define ICORE_H

#include <QObject>

#include "Global/CoreGlobal.h"

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class ActionSystem;
    class WindowSystem;
    class ICorePrivate;

    class CORE_EXPORT ICore : public QObject {
        Q_OBJECT
    public:
        static ICore *instance();

        ActionSystem *actionSystem() const;
        WindowSystem *windowSystem() const;

    private:
        explicit ICore(QObject *parent = nullptr);
        ~ICore();

        ICore(ICorePrivate &d, QObject *parent = nullptr);
        QScopedPointer<ICorePrivate> d_ptr;

        friend class Internal::CorePlugin;
    };

}

#endif // ICORE_H
