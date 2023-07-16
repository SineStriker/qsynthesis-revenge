#include "ILoader.h"
#include "ILoader_p.h"

#include <QDebug>
#include <QHash>

#include <QFile>
#include <QJsonDocument>

namespace Core {

    Q_GLOBAL_STATIC(QJsonObject, m_tempSettings)

    ILoaderPrivate::ILoaderPrivate() {
        settingsUnread = true;
        settingsNeedWrite = false;
    }

    ILoaderPrivate::~ILoaderPrivate() {
    }

    void ILoaderPrivate::init() {
    }

    bool ILoaderPrivate::readJson(const QString &filename, QJsonObject *out) {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        QByteArray data(file.readAll());
        file.close();

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            return false;
        }
        *out = doc.object();
        return true;
    }

    bool ILoaderPrivate::writeJson(const QString &filename, const QJsonObject &in) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            return false;
        }

        QJsonDocument doc;
        doc.setObject(in);
        file.write(doc.toJson(QJsonDocument::Compact));
        file.close();

        return true;
    }

    ILoader *m_instance = nullptr;

    Q_GLOBAL_STATIC_WITH_ARGS(QDateTime, m_atime, (QDateTime::currentDateTime()));

    ILoader::ILoader(QObject *parent) : ILoader(*new ILoaderPrivate(), parent) {
        m_instance = this;

        // Must initialize `m_atime`
        qDebug().noquote() << "ILoader: initialize at" << m_atime->toString("yyyy/MM/dd hh:mm:ss:zzz");

        connect(this, &ILoader::objectAdded, this, [&](const QString &id, QObject *obj) {
            qDebug().nospace() << "ILoader: object added (" << id << ", " << obj << ")"; //
        });

        connect(this, &ILoader::aboutToRemoveObject, this, [&](const QString &id, QObject *obj) {
            qDebug().nospace() << "ILoader: object to remove (" << id << ", " << obj << ")"; //
        });
    }

    ILoader::~ILoader() {
        Q_D(ILoader);
        if (d->settingsNeedWrite) {
            writeSettings();
        }

        m_instance = nullptr;
    }

    ILoader *ILoader::instance() {
        return m_instance;
    }

    QDateTime ILoader::atime() {
        return *m_atime;
    }

    QJsonObject *ILoader::tempSettings() {
        return m_tempSettings;
    }

    QString ILoader::settingsPath(QSettings::Scope scope) const {
        Q_D(const ILoader);
        return scope == QSettings::UserScope ? d->settingsPath : d->globalSettingsPath;
    }

    void ILoader::setSettingsPath(QSettings::Scope scope, const QString &path) {
        Q_D(ILoader);
        switch (scope) {
            case QSettings::UserScope:
                if (d->settingsPath != path) {
                    d->settingsPath = path;
                    d->settingsUnread = true;
                }
                break;
            case QSettings::SystemScope:
                if (d->globalSettingsPath != path) {
                    d->globalSettingsPath = path;
                    d->settingsUnread = true;
                }
                break;
            default:
                break;
        }
    }

    void ILoader::readSettings() {
        Q_D(ILoader);
        if (!d->settingsPath.isEmpty()) {
            ILoaderPrivate::readJson(d->settingsPath, &d->userSettings);
        }

        if (!d->globalSettingsPath.isEmpty()) {
            ILoaderPrivate::readJson(d->globalSettingsPath, &d->globalSettings);
        }

        d->settingsUnread = false;
    }

    void ILoader::writeSettings() const {
        Q_D(const ILoader);
        if (!d->settingsPath.isEmpty()) {
            ILoaderPrivate::writeJson(d->settingsPath, d->userSettings);
        }

        //        if (!d->globalSettingsPath.isEmpty()) {
        //            ILoaderPrivate::writeJson(d->globalSettingsPath, d->globalSettings);
        //        }

        d->settingsNeedWrite = false;
    }

    QJsonObject *ILoader::settings(QSettings::Scope scope) {
        Q_D(ILoader);

        QJsonObject *res = nullptr;

        if (d->settingsUnread) {
            d->userSettings = QJsonObject();
            d->globalSettings = QJsonObject();
            readSettings();
        }

        switch (scope) {
            case QSettings::UserScope:
                res = &d->userSettings;
                d->settingsNeedWrite = true;
                break;
            case QSettings::SystemScope:
                res = &d->globalSettings;
                d->settingsNeedWrite = true;
                break;
            default:
                break;
        }

        return res;
    }

    ILoader::ILoader(ILoaderPrivate &d, QObject *parent) : ObjectPool(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();
    }

}
