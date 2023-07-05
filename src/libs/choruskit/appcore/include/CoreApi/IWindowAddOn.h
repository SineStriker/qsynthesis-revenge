#ifndef IWINDOWADDON_H
#define IWINDOWADDON_H

#include <QObject>

#include "CkAppCoreGlobal.h"

namespace Core {

    class IWindow;
    class IWindowAddOn;
    class IWindowAddOnPrivate;

    class CKAPPCORE_API IWindowAddOn : public QObject {
        Q_OBJECT
    public:
        explicit IWindowAddOn(QObject *parent = nullptr);
        ~IWindowAddOn();

        IWindow *windowHandle() const;

    public:
        virtual void initialize() = 0;
        virtual void extensionsInitialized() = 0;
        virtual bool delayedInitialize();

    protected:
        IWindowAddOn(IWindowAddOnPrivate &d, QObject *parent = nullptr);

        QScopedPointer<IWindowAddOnPrivate> d_ptr;

        Q_DECLARE_PRIVATE(IWindowAddOn)

        friend class IWindow;
        friend class IWindowPrivate;
        friend class WindowSystem;
        friend class WindowSystemPrivate;
    };

}



#endif // IWINDOWADDON_H
