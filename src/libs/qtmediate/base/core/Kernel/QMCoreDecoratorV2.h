#ifndef QMCOREDECORATORV2_H
#define QMCOREDECORATORV2_H

#include <QObject>

#include "QMGlobal.h"

#define qIDec QMCoreDecoratorV2::instance()

class QMCoreDecoratorV2Private;

class QMCORE_EXPORT QMCoreDecoratorV2 : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMCoreDecoratorV2)
    Q_SINGLETON(QMCoreDecoratorV2)
public:
    explicit QMCoreDecoratorV2(QObject *parent = nullptr);
    ~QMCoreDecoratorV2();

public:
    void addTranslationPath(const QString &path);
    void removeTranslationPath(const QString &path);

    void installLocale(QObject *o, const std::function<void()> &updater);

public:
    QStringList locales() const;
    QString locale() const;
    void setLocale(const QString &locale);
    void refreshLocale();

signals:
    void localeChanged(const QString &locale);

protected:
    QMCoreDecoratorV2(QMCoreDecoratorV2Private &d, QObject *parent = nullptr);

    QScopedPointer<QMCoreDecoratorV2Private> d_ptr;
};

#endif // QMCOREDECORATORV2_H
