#ifndef QSCOREDECORATOR_H
#define QSCOREDECORATOR_H

#include <QLocale>
#include <QObject>

#include "QsMacros.h"
#include "QsGlobal.h"

#define qIDec QsCoreDecorator::instance()

class QsCoreDecoratorPrivate;

class QSBASE_API QsCoreDecorator : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsCoreDecorator)
    Q_SINGLETON(QsCoreDecorator)
public:
    QsCoreDecorator(QObject *parent = nullptr);
    ~QsCoreDecorator();

public:
    /**
     * @brief Current locale
     *
     * @return QLocale (No impact to framework locale)
     */
    QLocale locale() const;
    QList<QLocale> locales() const;
    void setLocale(const QLocale &locale);

    /**
     * @brief Add a translation configuration
     *
     * @param key Unique token
     * @param paths Locale and related qm file path
     */
    void addLocale(const QString &key, const QHash<QLocale, QStringList> &paths);
    void removeLocale(const QString &key);

    /**
     * @brief Install translation corresponding to the tokens to a widget, call at constructor
     *
     * @param w Widget pointer
     * @param keys Tokens of locales
     * @param updater Member function or global function to update texts
     */
    void installLocale(QObject *obj, const QStringList &keys,
                       const std::function<void()> updater = nullptr);
    void uninstallLocale(QObject *obj);

protected:
    QsCoreDecorator(QsCoreDecoratorPrivate &d, QObject *parent = nullptr);

    QScopedPointer<QsCoreDecoratorPrivate> d_ptr;

private:
    void _q_localeSubscriberDestroyed();
};

#endif // QSCOREDECORATOR_H