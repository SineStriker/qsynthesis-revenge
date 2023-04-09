#ifndef ICORE_H
#define ICORE_H

#include <QObject>

#include <CoreApi/ICoreBase.h>

#include "CoreGlobal.h"
#include "Preferences/PreferenceRegistry.h"

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class ICorePrivate;

    class CORE_EXPORT ICore : public ICoreBase {
        Q_OBJECT
    public:
        static ICore *instance();

        static QString mainTitle();
        static QString displayTitle(const QString &text);

        static void aboutApp(QWidget *parent = nullptr);

        static int showSettingsDialog(const QString &id, QWidget *parent = nullptr);
        static int showWizardDialog(const QString &id, QWidget *parent = nullptr);

        PreferenceRegistry *preferenceRegistry() const;

    private:
        explicit ICore(QObject *parent = nullptr);
        ~ICore();

        ICore(ICorePrivate &d, QObject *parent = nullptr);

        Q_DECLARE_PRIVATE(ICore)

        friend class Internal::CorePlugin;
    };

}

#endif // ICORE_H
