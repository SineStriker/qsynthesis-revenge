#ifndef ICORE_H
#define ICORE_H

#include <QObject>

#include <CoreApi/ActionSystem.h>
#include <CoreApi/DialogHelper.h>
#include <CoreApi/SettingCatalog.h>
#include <CoreApi/WindowSystem.h>

#include "CoreGlobal.h"
#include "Preferences/PreferenceRegistry.h"

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
        static int showSettingsDialog(const QString &id, QWidget *parent = nullptr);

        ActionSystem *actionSystem() const;
        WindowSystem *windowSystem() const;

        DialogHelper *dialogHelper() const;

        PreferenceRegistry *preferenceRegistry() const;

        SettingCatalog *settingCatalog() const;

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
