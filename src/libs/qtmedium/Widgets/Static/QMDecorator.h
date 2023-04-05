#ifndef QMDECORATOR_H
#define QMDECORATOR_H

#include <QScreen>

#include "QMCoreDecorator.h"
#include "QMWidgetsGlobal.h"

#ifdef qIDec
#    undef qIDec
#endif
#define qIDec qobject_cast<QMDecorator *>(QMCoreDecorator::instance())

class QMDecoratorPrivate;

class QMWIDGETS_API QMDecorator : public QMCoreDecorator {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMDecorator)
public:
    explicit QMDecorator(QObject *parent = nullptr);
    ~QMDecorator();

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

signals:
    void deviceRatioChanged(QScreen *screen, double dpi);
    void logicalRatioChanged(QScreen *screen, double dpi);

protected:
    QMDecorator(QMDecoratorPrivate &d, QObject *parent = nullptr);
};

#endif // QMDECORATOR_H
