#ifndef ICOREWINDOW_H
#define ICOREWINDOW_H

#include <QPointer>

#include <CoreApi/IWindow.h>
#include <Widgets/CommandPalette.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class ICoreWindowPrivate;

    class CORE_EXPORT ICoreWindow : public IWindow {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ICoreWindow)
    public:
        QMenuBar *menuBar() const override;
        void setMenuBar(QMenuBar *menuBar) override;

        QWidget *centralWidget() const override;
        void setCentralWidget(QWidget *widget) override;

        QStatusBar *statusBar() const override;
        void setStatusBar(QStatusBar *statusBar) override;

    public:
        QsApi::CommandPalette *commandPalette() const;

        Q_INVOKABLE void openFile(const QString &path);

        Q_INVOKABLE void openDirectory(const QString &path);

        Q_INVOKABLE void showAllActions();

        Q_INVOKABLE void selectColorThemes();

        Q_INVOKABLE void selectTranslations();

        Q_INVOKABLE void selectRecentFiles(bool dirsAtTop = false);

        Q_INVOKABLE void showMenuInPalette(QMenu *menu, bool deleteLater);

    protected:
        ICoreWindow(const QString &id, QObject *parent = nullptr);
        ~ICoreWindow();

        QString correctWindowTitle(const QString &title) const override;

        QWidget *createWindow(QWidget *parent) const override;

        void setupWindow() override;
        void windowAddOnsInitialized() override;
        void windowAddOnsFinished() override;
        void windowClosed() override;

        void actionItemAdded(ActionItem *item) override;
        void actionItemRemoved(ActionItem *item) override;

    protected:
        ICoreWindow(ICoreWindowPrivate &d, const QString &id, QObject *parent = nullptr);
    };

}



#endif // ICOREWINDOW_H
