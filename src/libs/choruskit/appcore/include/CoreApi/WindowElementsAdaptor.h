#ifndef WINDOWELEMENTSADAPTOR_H
#define WINDOWELEMENTSADAPTOR_H

#include <QCoreApplication>
#include <QWidget>

#include "CkAppCoreGlobal.h"

class QMenuBar;
class QStatusBar;

namespace Core {

    class WindowElementsAdaptorPrivate;

    class CKAPPCORE_API WindowElementsAdaptor {
        Q_DECLARE_TR_FUNCTIONS(WindowElementsAdaptor)
    public:
        explicit WindowElementsAdaptor(QWidget *w = nullptr);
        ~WindowElementsAdaptor();

        QWidget *window() const;

        bool windowModified() const;
        void setWindowModified(bool modified);

        bool windowFileReadOnly() const;
        void setWindowFileReadOnly(bool readOnly);

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
