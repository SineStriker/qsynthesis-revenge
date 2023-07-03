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
        void loadInvisibleContext();

        ActionContext *mainMenuCtx;
        ActionContext *invisibleCtx;

        QMenu *invisibleCtxMenu;

        CommandPalette *cp;

        void showAllActions_helper();
        void selectEditor_helper(QList<DocumentSpec *> &specs, const QString &path);
        void selectColorThemes_helper();
        void selectTranslations_helper();
        void selectRecentFiles_helper(bool dirsAtTop);
        void showMenuInPalette_helper(QMenu *menu, QMenu *menuToDelete);

        void openEditor(DocumentSpec *spec, const QString &path);

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
    };

}


#endif // ICOREWINDOW_P_H
