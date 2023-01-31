#ifndef CDECORATOR_H
#define CDECORATOR_H

#include <QLocale>
#include <QObject>
#include <QScreen>

#include "QsFrameworkGlobal.h"
#include "QsMacros.h"

#define qIDec CDecorator::instance()

class CDecoratorPrivate;

class CDecorator : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CDecorator)
    Q_SINGLETON(CDecorator)
public:
    explicit CDecorator(QObject *parent = nullptr);
    ~CDecorator();

    /**
     * @brief Current locale
     *
     * @return QLocale (No impact to framework locale)
     */
    QLocale locale() const;
    void setLocale(const QLocale &locale);

    /**
     * @brief Add a translation configuration
     *
     * @param key Unique token
     * @param paths Locale and related qm file path
     */
    void addLocale(const QString &key, const QMap<QLocale, QStringList> &paths);
    void removeLocale(const QString &key);

    /**
     * @brief Install translation corresponding to the tokens to a widget, call at constructor
     *
     * @param w Widget pointer
     * @param keys Tokens of locales
     * @param updater Member function or global function to update texts
     */
    void installLocale(QWidget *w, const QStringList &keys,
                       const std::function<void()> updater = nullptr);
    void uninstallLocale(QWidget *w);

    /**
     * @brief Current theme
     *
     * @return Theme string
     */
    QString theme() const;
    void setTheme(const QString &theme);

    /**
     * @brief Add a stylesheet template file (Persistent)
     *
     * @param key Unique token
     * @param path qss.in file path
     */
    void addThemeTemplate(const QString &key, const QString &path);
    void removeThemeTemplate(const QString &key);

    /**
     * @brief Add a theme configuration
     *
     * @param key Unique token
     * @param paths Theme and related json file path
     */
    void addThemeConfig(const QString &key, const QMap<QString, QStringList> &paths);
    void removeThemeConfig(const QString &key);

    /**
     * @brief Install themes corresponding to the templates to a widget, call at constructor
     *
     * @param w Widget pointer
     * @param keys Template defined in qss.in file
     */
    void installTheme(QWidget *w, const QStringList &templateKeys);
    void uninstallTheme(QWidget *w);
    
protected:
    CDecorator(CDecoratorPrivate &d, QObject *parent = nullptr);

    QScopedPointer<CDecoratorPrivate> d_ptr;

private:
    void _q_screenAdded(QScreen *screen);
    void _q_screenRemoved(QScreen *screen);
    void _q_deviceRatioChanged(double dpi);
    void _q_logicalRatioChanged(double dpi);

    void _q_localeSubscriberDestroyed();
    void _q_themeSubscriberDestroyed();

signals:
    void deviceRatioChanged(QScreen *screen, double dpi);
    void logicalRatioChanged(QScreen *screen, double dpi);
};

#endif // CDECORATOR_H
