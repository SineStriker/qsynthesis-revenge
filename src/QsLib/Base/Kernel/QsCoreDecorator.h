#ifndef QSCOREDECORATOR_H
#define QSCOREDECORATOR_H

#include <QObject>

#include "QsGlobal.h"
#include "QsMacros.h"

#define qIDec QsCoreDecorator::instance()

class QsCoreDecoratorPrivate;

class QSBASE_API QsCoreDecorator : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QsCoreDecorator)
    Q_SINGLETON(QsCoreDecorator)
public:
    explicit QsCoreDecorator(QObject *parent = nullptr);
    ~QsCoreDecorator();

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
    QsCoreDecorator(QsCoreDecoratorPrivate &d, QObject *parent = nullptr);

    QScopedPointer<QsCoreDecoratorPrivate> d_ptr;

private:
    void _q_localeSubscriberDestroyed();
};

#ifndef _LOC
#define _LOC(T, P) std::bind(&T::reloadStrings, P)
#endif

#endif // QSCOREDECORATOR_H
