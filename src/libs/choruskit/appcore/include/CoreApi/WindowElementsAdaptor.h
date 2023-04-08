#ifndef WINDOWELEMENTSADAPTOR_H
#define WINDOWELEMENTSADAPTOR_H

#include "CkCoreGlobal.h"

#include <QWidget>

class QMenuBar;
class QStatusBar;

namespace Core {

    class WindowElementsAdaptorPrivate;

    class CKCORE_API WindowElementsAdaptor {
    public:
        explicit WindowElementsAdaptor(QWidget *w = nullptr);
        ~WindowElementsAdaptor();

        QWidget *window() const;

        bool windowModified() const;
        void setWindowModified(bool modified);

        QString windowFilePath() const;
        void setWindowFilePath(const QString &path);

        QString windowTitle() const;
        void setWindowTitle(const QString &title);

    public:
        virtual QMenuBar *menuBar() const;
        virtual void setMenuBar(QMenuBar *menuBar);

        virtual QWidget *centralWidget() const;
        virtual void setCentralWidget(QWidget *widget);

        virtual QStatusBar *statusBar() const;
        virtual void setStatusBar(QStatusBar *statusBar);

    protected:
        void setWindow(QWidget *w);

        virtual QString modifiedIdentifier() const;
        virtual QString correctWindowTitle(const QString &title) const;

    protected:
        QScopedPointer<WindowElementsAdaptorPrivate> d;

        friend class WindowElementsAdaptorPrivate;
    };

}

#endif // WINDOWELEMENTSADAPTOR_H
