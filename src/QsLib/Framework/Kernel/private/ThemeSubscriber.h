#ifndef THEMESUBSCRIBER_H
#define THEMESUBSCRIBER_H

#include <QScreen>
#include <QWidget>

#include "../CDecorator.h"
#include "ThemeTemplate.h"

class ThemeSubscriber : public QObject {
    Q_OBJECT
public:
    ThemeSubscriber(QWidget *w, CDecorator *dec, CDecoratorPrivate *decp);
    ~ThemeSubscriber();

    CDecorator *dec;
    CDecoratorPrivate *decp;

    QWidget *w;
    QWindow *winHandle;

    QHash<QString, QSharedPointer<ThemeTemplate>> templates;

    void updateScreen();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void _q_screenChanged(QScreen *screen);
    void _q_deviceRatioChanged(QScreen *screen, double dpi);
    void _q_logicalRatioChanged(QScreen *screen, double dpi);
};

#endif // THEMESUBSCRIBER_H
