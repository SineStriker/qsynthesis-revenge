#ifndef THEMEGUARD_H
#define THEMEGUARD_H

#include <QScreen>
#include <QWidget>

#include "../CDecorator.h"
#include "ThemeTemplate.h"

struct ThemeSubscriber;

class ThemeGuard : public QObject {
    Q_OBJECT
public:
    ThemeGuard(QWidget *w, CDecorator *dec, CDecoratorPrivate *decp, ThemeSubscriber *g);
    ~ThemeGuard();

    // Object pointers
    CDecorator *dec;
    CDecoratorPrivate *decp;

    bool needUpdate;
    ThemeSubscriber *group;

    QWidget *w;
    QWindow *winHandle;

    void updateScreen();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void _q_screenChanged(QScreen *screen);
    void _q_deviceRatioChanged(QScreen *screen, double dpi);
    void _q_logicalRatioChanged(QScreen *screen, double dpi);
};

#endif // THEMEGUARD_H
