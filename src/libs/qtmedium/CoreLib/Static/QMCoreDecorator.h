#ifndef QMCOREDECORATOR_H
#define QMCOREDECORATOR_H

#include <QObject>

#include "QMGlobal.h"

#define qIDec QMCoreDecorator::instance()

class QMCoreDecoratorPrivate;

class QMCORELIB_API QMCoreDecorator : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMCoreDecorator)
    Q_SINGLETON(QMCoreDecorator)
public:
    explicit QMCoreDecorator(QObject *parent = nullptr);
    ~QMCoreDecorator();

public:
    /**
     * @brief Current theme
     *
     * @return Theme string
     */
    QString theme() const;
    QStringList themes() const;
    virtual void setTheme(const QString &theme);

    /**
     * @brief Current locale
     *
     * @return Locale name (No impact to framework locale)
     */
    QString locale() const;
    QStringList locales() const;
    virtual void setLocale(const QString &locale);

    /**
     * @brief Add a translation configuration
     *
     * @param key Unique token
     * @param paths Locale and related qm file path
     */
    void addLocale(const QString &key, const QMap<QString, QStringList> &paths);
    void removeLocale(const QString &key);

    /**
     * @brief Install translation corresponding to the tokens to a widget, call at constructor
     *
     * @param w Widget pointer
     * @param keys Tokens of locales
     * @param updater Member function or global function to update texts
     */
    void installLocale(QObject *obj, const QStringList &keys, const std::function<void()> updater);
    void uninstallLocale(QObject *obj);

protected:
    QMCoreDecorator(QMCoreDecoratorPrivate &d, QObject *parent = nullptr);

    QScopedPointer<QMCoreDecoratorPrivate> d_ptr;

private:
    void _q_localeSubscriberDestroyed();
};

#ifndef _LOC
#    define _LOC(T, P) std::bind(&T::reloadStrings, P)
#endif

#endif // QMCOREDECORATOR_H
