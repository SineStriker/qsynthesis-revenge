#ifndef ICORE_H
#define ICORE_H

#include <QObject>

#include <CoreApi/ActionSystem.h>
#include <CoreApi/WindowSystem.h>

#include "Settings/SettingCatalog.h"

#include <CoreApi/DialogHelper.h>

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

        static QString mainTitle();
        static QString displayTitle(const QString &text);
        static void aboutApp(QWidget *parent = nullptr);

        ActionSystem *actionSystem() const;
        WindowSystem *windowSystem() const;

        DialogHelper *dialogHelper() const;

        // SettingCatalog *settingCatalog() const;

    private:
        explicit ICore(QObject *parent = nullptr);
        ~ICore();

        ICore(ICorePrivate &d, QObject *parent = nullptr);
        QScopedPointer<ICorePrivate> d_ptr;

        Q_DECLARE_PRIVATE(ICore)

        friend class Internal::CorePlugin;
    };

}

#endif // ICORE_H
