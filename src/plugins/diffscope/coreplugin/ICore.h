#ifndef ICORE_H
#define ICORE_H

#include <QObject>
#include <QSettings>

#include <CoreApi/ICoreBase.h>

#include "CoreGlobal.h"

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class ICorePrivate;

    class CORE_EXPORT ICore : public ICoreBase {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ICore)
    public:
        static ICore *instance();

        /*
         * brief: Return title `ChorusKit DiffScope`
         */
        static QString mainTitle();

        /*
         * brief: Return title `[text] - ChorusKit DiffScope`
         */
        static QString displayTitle(const QString &text);

        /*
         * brief: Show message box about this application
         */
        static void aboutApp(QWidget *parent = nullptr);

        /*
         * brief: Open settings dialog with initial page specified by `id`
         */
        static int showSettingsDialog(const QString &id, QWidget *parent = nullptr);

        /*
         * brief Open HomeWindow instance
         */
        static void showHome();

    public:
        /**
         * brief: Automatically set any child popup menu's style property to update its style
         *
         * @param w Widget that may have context menu
         */
        static void autoPolishPopupMenu(QWidget *w);

        /**
         * brief: Create a menu with core style
         *
         * @param parent argument for constructor
         * @return QMenu instance
         */
        static QMenu *createCoreMenu(QWidget *parent = nullptr);

    private:
        explicit ICore(QObject *parent = nullptr);
        ~ICore();

        ICore(ICorePrivate &d, QObject *parent = nullptr);

        friend class Internal::CorePlugin;
    };

}

#endif // ICORE_H
