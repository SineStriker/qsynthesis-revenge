#ifndef THEMEGUARD_H
#define THEMEGUARD_H

#include <QScreen>
#include <QWidget>

#include "../CDecorator.h"
#include "ThemeTemplate.h"

struct ScreenSet;
struct ThemeSubscriber;

class ThemeGuard : public QObject {
    Q_OBJECT
public:
    ThemeGuard(QWidget *w, ThemeSubscriber *g);
    ~ThemeGuard();

    QWidget *w;
    QWindow *winHandle;

    // Object pointers
    bool needUpdate;
    ThemeSubscriber *group;
    ScreenSet *screenSet;

    std::list<ThemeGuard *>::iterator queueIterator;

    bool updateScreen();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void _q_screenChanged(QScreen *screen);
};

#endif // THEMEGUARD_H
