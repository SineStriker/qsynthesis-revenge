#ifndef QMDECORATORV2_H
#define QMDECORATORV2_H

#include <QScreen>
#include <QTimer>

#include "QMGuiDecoratorV2.h"
#include "QMWidgetsGlobal.h"

#ifdef qIDec
#    undef qIDec
#endif
#define qIDec qobject_cast<QMDecoratorV2 *>(QMCoreDecoratorV2::instance())

class QMDecoratorV2Private;

class QMWIDGETS_EXPORT QMDecoratorV2 : public QMGuiDecoratorV2 {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMDecoratorV2)
public:
    explicit QMDecoratorV2(QObject *parent = nullptr);
    ~QMDecoratorV2();

    static QString evaluateStyleSheet(const QString &stylesheet, double ratio = 1);

public:
    void addThemePath(const QString &path);
    void removeThemePath(const QString &path);

    void installTheme(QWidget *w, const QString &id);

public:
    QStringList themes() const;
    QString theme() const;
    void setTheme(const QString &theme);
    void refreshTheme();

    QString themeVariable(const QString &key) const;

signals:
    void themeChanged(const QString &theme);

protected:
    QMDecoratorV2(QMDecoratorV2Private &d, QObject *parent = nullptr);
};

#ifndef _LOC
#    define _LOC(T, P) std::bind(&T::reloadStrings, P)
#endif

#endif // QMDECORATORV2_H
