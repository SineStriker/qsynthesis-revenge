#ifndef ICOREWINDOW_H
#define ICOREWINDOW_H

#include "Window/IWindow.h"

namespace Core {

    namespace Internal {

        class ICoreWindowFactory : public IWindowFactory {
        public:
            explicit ICoreWindowFactory(const QString &id);
            ~ICoreWindowFactory();

            IWindow *create(QObject *parent) override;
            IWindow *create(const QString &id, QObject *parent) override;
        };

        class ICoreWindow : public IWindow {
            Q_OBJECT
        protected:
            explicit ICoreWindow(const QString &id, QObject *parent = nullptr);
            ~ICoreWindow();

        public:
            QMenuBar *menuBar() const override;
            void setMenuBar(QMenuBar *menuBar) override;

            QWidget *centralWidget() const override;
            void setCentralWidget(QWidget *widget) override;

            QStatusBar *statusBar() const override;
            void setStatusBar(QStatusBar *statusBar) override;

        protected:
            QWidget *createWindow(QWidget *parent) const override;

            friend class ICoreWindowFactory;
        };

    }

}



#endif // ICOREWINDOW_H
