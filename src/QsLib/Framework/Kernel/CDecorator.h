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
        China = QLocale::China,
        HongKong = QLocale::HongKong,
        Japan = QLocale::Japan,
        UnitedStates = QLocale::UnitedStates,
    };

    enum Theme {
        Light,
        Dark,
        MultiColor,
        UserTheme = 100,
    };

    // Locale related
    int locale() const;
    void setLocale(int locale);

    void addLocale(const QString &key, const QMap<int, QStringList> &qm);
    void removeLocale(const QString &key);

    // Theme Related
    int theme() const;
    void setTheme(int theme);

    void addTheme(const QString &key, const QStringList &qss, const QMap<int, QStringList> &conf);
    void removeTheme(const QString &key);

    /**
     * @brief Call at constructor
     *
     * @tparam T Derived from QWidget
     * @param w Widget pointer
     * @param key Unique key of locale
     */
    template <class T>
    void installLocale(T *w) {
        static_assert(std::is_base_of<QWidget, T>::value, "T must derive from QWidget");
        // First translation
        w->reloadStrings(locale(), QString());
        // Listen to global manager
        connect(qIDec, &CDecorator::localeChanged, this, &T::reloadStrings);
    }

    /**
     * @brief Call at constructor
     *
     * @param w Widget pointer
     * @param namespaces Namespaces defined in qss.in file
     */
    void installTheme(QWidget *w, const QStringList &namespaces);

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

signals:
    void themeChanged(int theme, const QString &key);
    void localeChanged(int locale, const QString &key);

    void deviceRatioChanged(QScreen *screen, double dpi);
    void logicalRatioChanged(QScreen *screen, double dpi);
};

#endif // CDECORATOR_H
