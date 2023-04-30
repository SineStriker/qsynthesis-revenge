#ifndef IWINDOW_H
#define IWINDOW_H

#include "ActionItem.h"
#include "ObjectPool.h"
#include "WindowElementsAdaptor.h"

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class IWindow;
    class IWindowFactoryPrivate;

    class CKAPPCORE_API IWindowFactory {
    public:
        enum AvailableCreator {
            Create,
            CreateWithId,
        };

        explicit IWindowFactory(const QString &id, AvailableCreator creator);
        virtual ~IWindowFactory();

        QString id() const;
        AvailableCreator creator() const;

        virtual IWindow *create(QObject *parent);
        virtual IWindow *create(const QString &id, QObject *parent);

    private:
        QScopedPointer<IWindowFactoryPrivate> d_ptr;
    };

    class IWindowPrivate;

    class CKAPPCORE_API IWindow : public ObjectPool, public WindowElementsAdaptor {
        Q_OBJECT
    public:
        QString id() const;

        void addWidget(const QString &id, QWidget *w);
        void removeWidget(const QString &id);
        QWidget *widget(const QString &id) const;
        QList<QWidget *> widgets() const;

        void addActionItem(ActionItem *item);
        void addActionItems(const QList<ActionItem *> &items);
        void removeActionItem(ActionItem *item);
        void removeActionItem(const QString &id);
        ActionItem *actionItem(const QString &id) const;
        QList<ActionItem *> actionItems() const;

        void reloadActions();

        bool hasDragFileHandler(const QString &suffix);
        void setDragFileHandler(const QString &suffix, QObject *obj, const char *member, int maxCount = 0);
        void removeDragFileHandler(const QString &suffix);

    signals:
        void widgetAdded(const QString &id, QWidget *w);
        void aboutToRemoveWidget(const QString &id, QWidget *w);

        void aboutToClose();
        void closed();

    protected:
        explicit IWindow(const QString &id, QObject *parent = nullptr);
        ~IWindow();

        virtual QWidget *createWindow(QWidget *parent) const = 0;

        virtual void setupWindow();
        virtual void windowAddOnsFinished();

        virtual void windowAboutToClose();
        virtual void windowClosed();

    protected:
        IWindow(IWindowPrivate &d, const QString &id, QObject *parent = nullptr);
        QScopedPointer<IWindowPrivate> d_ptr;

        Q_DECLARE_PRIVATE(IWindow)

        friend class ICore;
        friend class ICorePrivate;
        friend class Internal::CorePlugin;
        friend class WindowSystem;
        friend class WindowSystemPrivate;
        friend class IWindowFactory;

    public:
        bool isEffectivelyClosed() const;

        template <class T>
        inline T *cast();

        template <class T>
        inline const T *cast() const;
    };

    template <class T>
    inline T *IWindow::cast() {
        static_assert(std::is_base_of<IWindow, T>::value, "T should inherit from Core::IWindow");
        return qobject_cast<T *>(this);
    }

    template <class T>
    inline const T *IWindow::cast() const {
        static_assert(std::is_base_of<IWindow, T>::value, "T should inherit from Core::IWindow");
        return qobject_cast<T *>(this);
    }

}

#endif // IWINDOW_H
