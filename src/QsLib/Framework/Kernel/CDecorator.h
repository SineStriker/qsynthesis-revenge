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
    CDecorator(QObject *parent = nullptr);
    ~CDecorator();

    enum Locale {
        China = QLocale::China,               // 简体中文
        HongKong = QLocale::HongKong,         // 繁體中文
        Japan = QLocale::Japan,               // 日本語
        UnitedStates = QLocale::UnitedStates, // English (US)
    };

    enum Theme {
        Light,
        Dark,
        MultiColor,

        // User
        UserTheme = 100,
    };

    /**
     * @brief Current locale
     *
     * @return Locale enumeration
     */
    int locale() const;
    void setLocale(int locale);

    /**
     * @brief Add a translation configuration
     *
     * @param key Unique token
     * @param qm Locale and related qm file path
     */
    void addLocale(const QString &key, const QMap<int, QStringList> &qm);
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
     * @return Theme enumeration
     */
    int theme() const;
    void setTheme(int theme);

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
     * @param conf Theme and related json file path
     */
    void addThemeConfig(const QString &key, const QMap<int, QStringList> &conf);
    void removeThemeConfig(const QString &key);

    /**
     * @brief Install themes corresponding to the namespaces to a widget, call at constructor
     *
     * @param w Widget pointer
     * @param namespaces Namespaces defined in qss.in file
     */
    void installTheme(QWidget *w, const QStringList &keys);
    void uninstallTheme(QWidget *w);

    /**
     * @brief Register a new style type
     *
     * @return New theme enumeration >= Theme::UserTheme
     */
    int registerNewTheme();

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
