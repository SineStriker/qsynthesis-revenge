#ifndef QMCOREAPPEXTENSION_H
#define QMCOREAPPEXTENSION_H

#include <QLoggingCategory>
#include <QObject>
#include <QSettings>

#include "QMGlobal.h"

#define qAppExt QMCoreAppExtension::instance()

class QMCoreAppExtensionPrivate;

class QMCORE_EXPORT QMCoreAppExtension : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMCoreAppExtension)
    Q_SINGLETON(QMCoreAppExtension)
public:
    explicit QMCoreAppExtension(QObject *parent = nullptr);
    ~QMCoreAppExtension();

public:
    /**
     * @brief Tell if QCoreApplication::quit() is emitted
     *
     */
    bool isAboutToQuit() const;

    /**
     * @brief Application data path, default to follows
     *        Mac/Linux:     %Home%/.config/ChorusKit/%AppName%
     *        Windows:       %UserProfile%/AppData/Local/%AppName%
     */
    QString appDataDir() const;
    void setAppDataDir(const QString &dir);

    /**
     * @brief Application temporary path, default to follows
     *        Mac/Linux:     %TMPDIR%
     *        Windows:       %TEMP%
     */
    QString tempDir() const;
    void setTempDir(const QString &dir);

    /**
     * @brief Global library path, default to follows
     *        Mac:           %AppPath%/../Frameworks
     *        Windows/Linux: %AppPath%/../lib
     */
    QString libDir() const;
    void setLibDir(const QString &dir);

    /**
     * @brief Global share path, default to follows
     *        Mac:           %AppPath%/../Resources
     *        Windows/Linux: %AppPath%/../share
     */
    QString shareDir() const;
    void setShareDir(const QString &dir);

    /**
     * @brief Application share path, default to follows
     *        Mac:           %SharePath%/%AppName%
     *        Windows/Linux: %SharePath%
     */
    QString appShareDir() const;
    void setAppShareDir(const QString &dir);

    /**
     * @brief Application plugins path, default to follows
     *        Mac:           %LibPath%/%AppName%/plugins
     *        Windows/Linux: %AppPath%/../Plugins
     */
    QString appPluginsDir() const;
    void setAppPluginsDir(const QString &dir);

    /**
     * @brief Create data and temp directories for further use
     *
     * @return true  Success
     * @return false Failed
     */
    bool createDataAndTempDirs() const;

public:
    static QString configurationPath(QSettings::Scope scope = QSettings::UserScope);

    static QString configurationBasePrefix();

protected:
    QMCoreAppExtension(QMCoreAppExtensionPrivate &d, QObject *parent = nullptr);

    QScopedPointer<QMCoreAppExtensionPrivate> d_ptr;
};

QMCORE_EXPORT Q_DECLARE_LOGGING_CATEGORY(qAppExtLog)

#endif // QMCOREAPPEXTENSION_H
