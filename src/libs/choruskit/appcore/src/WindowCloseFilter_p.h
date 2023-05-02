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
        void actionChanged(QWidget *w);

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;
    };

}

#endif // WINDOWCLOSEFILTER_P_H
