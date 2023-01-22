#ifndef LOCALDECORATOR_H
#define LOCALDECORATOR_H

#include <QHash>
#include <QObject>
#include <QWidget>

#include "QsFrameworkGlobal.h"

class LocalDecoratorPrivate;

class QSFRAMEWORK_API LocalDecorator : public QObject {
    Q_OBJECT
public:
    explicit LocalDecorator(QWidget *parent = nullptr);
    ~LocalDecorator();

    void reloadScreen(int theme);

public:
    void addTheme(int theme, const QStringList &paths);
    void removeTheme(int theme);

protected:
    QScopedPointer<LocalDecoratorPrivate> d_ptr;

    bool eventFilter(QObject *obj, QEvent *event) override;

    friend class LocalDecoratorPrivate;

private:
    void _q_screenChanged(QScreen *screen);
    void _q_deviceRatioChanged(QScreen *screen, double dpi);
    void _q_logicalRatioChanged(QScreen *screen, double dpi);
};

#endif // LOCALDECORATOR_H
