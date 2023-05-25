#ifndef ICOREWINDOW_P_H
#define ICOREWINDOW_P_H

#include <QSet>

#include <Widgets/CommandPalette.h>

#include <CoreApi/ActionContext.h>
#include <CoreApi/DocumentSpec.h>
#include <CoreApi/IWindow_p.h>

#include "ICoreWindow.h"

namespace Core {

    class ICoreWindowPrivate : public IWindowPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ICoreWindow)
    public:
        ICoreWindowPrivate();
        ~ICoreWindowPrivate();

        void init();

        void reloadMenuBar();

        ActionContext *mainMenuCtx;

        QsApi::CommandPalette *cp;

        void showAllActions_helper();
        void selectEditor_helper(QList<DocumentSpec *> &specs, const QString &path);
        void openEditor(DocumentSpec *spec, const QString &path);
    };

}


#endif // ICOREWINDOW_P_H
