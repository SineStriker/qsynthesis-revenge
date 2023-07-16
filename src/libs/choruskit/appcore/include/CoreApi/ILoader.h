#ifndef ILOADER_H
#define ILOADER_H

#include <QDateTime>
#include <QJsonObject>
#include <QSettings>
#include <QSplashScreen>

#include "ObjectPool.h"

namespace Core {

    class ILoaderPrivate;

    class CKAPPCORE_API ILoader : public ObjectPool {
        Q_OBJECT
        Q_DECLARE_PRIVATE(ILoader);

    public:
        explicit ILoader(QObject *parent = nullptr);
        ~ILoader();

        static ILoader *instance();

        static QDateTime atime();

        static QJsonObject *tempSettings();

    public:
        QString settingsPath(QSettings::Scope scope) const;
        void setSettingsPath(QSettings::Scope scope, const QString &path);

        void readSettings();
        void writeSettings() const;

        QJsonObject *settings(QSettings::Scope scope = QSettings::UserScope);

    protected:
        ILoader(ILoaderPrivate &d, QObject *parent = nullptr);

        QScopedPointer<ILoaderPrivate> d_ptr;
    };

    class CKAPPCORE_API InitialRoutine : public QObject {
        Q_OBJECT
    public:
        explicit InitialRoutine(QSplashScreen *screen, QObject *parent = nullptr) : QObject(parent), m_screen(screen) {
        }

    public:
        inline QSplashScreen *splash() const {
            return m_screen;
        }

        inline QList<std::function<void()>> &routines() {
            return m_routines;
        }

    signals:
        void done();

    protected:
        QSplashScreen *m_screen;
        QList<std::function<void()>> m_routines;
    };

}


#endif // ILOADER_H
