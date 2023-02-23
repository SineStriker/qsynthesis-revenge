#ifndef CDECORATOR_H
#define CDECORATOR_H

#include <QScreen>

#include "QMWidgetsGlobal.h"
#include "QsCoreDecorator.h"

#ifdef qIDec
#    undef qIDec
#endif
#define qIDec qobject_cast<CDecorator *>(QsCoreDecorator::instance())

class CDecoratorPrivate;

class QMWIDGETS_API CDecorator : public QsCoreDecorator {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CDecorator)
public:
    explicit CDecorator(QObject *parent = nullptr);
    ~CDecorator();

    void setTheme(const QString &theme) override;

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

private:
    void _q_screenAdded(QScreen *screen);
    void _q_screenRemoved(QScreen *screen);
    void _q_deviceRatioChanged(double dpi);
    void _q_logicalRatioChanged(double dpi);

    void _q_themeSubscriberDestroyed();

signals:
    void deviceRatioChanged(QScreen *screen, double dpi);
    void logicalRatioChanged(QScreen *screen, double dpi);
};

#endif // CDECORATOR_H
