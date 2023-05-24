#ifndef WINDOWCLOSEFILTER_P_H
#define WINDOWCLOSEFILTER_P_H

#include <QWidget>

namespace Core {

    class IWindowPrivate;

    class WindowCloseFilter : public QObject {
        Q_OBJECT
    public:
        explicit WindowCloseFilter(IWindowPrivate *d, QWidget *w);
        ~WindowCloseFilter();

        IWindowPrivate *d;
        QWidget *w;

    signals:
        void windowClosed(QWidget *w);

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
    };

    class WindowActionFilter : public QObject {
        Q_OBJECT
    public:
        explicit WindowActionFilter(IWindowPrivate *d);
        ~WindowActionFilter();

        IWindowPrivate *d;

    signals:
        void actionChanged(QWidget *w, int type, QAction *action);

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
    };

    class ShortcutFilter : public QObject {
        Q_OBJECT
    public:
        explicit ShortcutFilter(IWindowPrivate *d);
        ~ShortcutFilter();

        IWindowPrivate *d;

    signals:
        void shortcutAboutToCome(QAction *action);

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
    };

}

#endif // WINDOWCLOSEFILTER_P_H
