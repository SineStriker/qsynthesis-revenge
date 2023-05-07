#ifndef IIWindowContext_H
#define IIWindowContext_H

#include <QDateTime>

#include <CoreApi/IWindow.h>

namespace Core {

    class IWindowContextPrivate;

    class CKAPPCORE_API IWindowContext : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IWindowContext)
    public:
        explicit IWindowContext(Core::IWindow *winHandle, QObject *parent = nullptr);
        ~IWindowContext();

    public:
        Core::IWindow *windowHandle() const;

        QDateTime dateTime() const;

    protected:
        IWindowContext(IWindowContextPrivate &d, Core::IWindow *winHandle, QObject *parent = nullptr);

        QScopedPointer<IWindowContextPrivate> d_ptr;
    };

}

#endif // IIWindowContext_H