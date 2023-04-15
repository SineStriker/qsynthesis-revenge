#ifndef CHORUSKIT_ISTYLEHELPER_H
#define CHORUSKIT_ISTYLEHELPER_H

#include <QMenu>

#include "CoreGlobal.h"

namespace Core {

    namespace Internal {
        class CorePlugin;
    }

    class IStyleHelperPrivate;

    class CORE_EXPORT IStyleHelper :public QObject{
        Q_OBJECT
        Q_DECLARE_PRIVATE(IStyleHelper)
    public:
        /**
         * brief: Automatically set any child popup menu's style property to update its style
         *
         * @param w Widget that may have context menu
         */
        static void autoPolishPopupMenu(QWidget *w);

        /**
         * brief: Create a menu with style property set
         *
         * @param parent argument for constructor
         * @return QMenu instance
         */
        static QMenu *createPolishedMenu(QWidget *parent = nullptr);

    private:
        explicit IStyleHelper(QObject *parent = nullptr);
        ~IStyleHelper();

        IStyleHelper(IStyleHelperPrivate &d, QObject *parent = nullptr);

        QScopedPointer<IStyleHelperPrivate> d_ptr;

        friend class Internal::CorePlugin;
    };

} // Core

#endif // CHORUSKIT_ISTYLEHELPER_H
