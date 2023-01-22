#ifndef CDECORATORPRIVATE_H
#define CDECORATORPRIVATE_H

#include "../CDecorator.h"

#include <QHash>

class CDecoratorFilter : public QObject {
    Q_OBJECT
public:
    CDecoratorFilter(QWidget *w, CDecorator *dec, CDecoratorPrivate *decp);
    ~CDecoratorFilter();

    CDecorator *dec;
    CDecoratorPrivate *decp;

    QWidget *w;
    QWindow *winHandle;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void _q_screenChanged(QScreen *screen);
    void _q_deviceRatioChanged(QScreen *screen, double dpi);
    void _q_logicalRatioChanged(QScreen *screen, double dpi);
};

class CDecoratorPrivate {
    Q_DECLARE_PUBLIC(CDecorator)
public:
    CDecoratorPrivate();
    virtual ~CDecoratorPrivate();

    void init();

    CDecorator *q_ptr;

    int theme;
    int loc;

    struct ThemeData {
        CDecoratorFilter *filter;
    };

    struct LocaleData {};

    QHash<QString, ThemeData> themes;
    QHash<QString, LocaleData> locales;

    int themeMax;
};

#endif // CDECORATORPRIVATE_H
