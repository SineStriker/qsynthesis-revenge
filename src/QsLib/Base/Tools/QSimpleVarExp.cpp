#include "QSimpleVarExp.h"

#include "QsSystem.h"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

static const char Slash = '/';

QSimpleVarExp::QSimpleVarExp() : QSimpleVarExp("\\$\\{(\\w+)\\}") {
}

QSimpleVarExp::QSimpleVarExp(const QString &pattern) : Pattern(pattern) {
}

QSimpleVarExp::~QSimpleVarExp() {
}

void QSimpleVarExp::addJsonObject(const QJsonObject &obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        if (it->isString()) {
            Variables.insert(it.key(), it->toString());
        }
    }
}

void QSimpleVarExp::addVariantMap(const QVariantMap &map) {
    for (auto it = map.begin(); it != map.end(); ++it) {
        if (it->type() == QVariant::String) {
            Variables.insert(it.key(), it->toString());
        }
    }
}

void QSimpleVarExp::addMap(const QMap<QString, QString> &map) {
    for (auto it = map.begin(); it != map.end(); ++it) {
        Variables.insert(it.key(), it.value());
    }
}

void QSimpleVarExp::addHash(const QHash<QString, QString> &hash) {
    for (auto it = hash.begin(); it != hash.end(); ++it) {
        Variables.insert(it.key(), it.value());
    }
}

void QSimpleVarExp::add(const QString &key, const QString &value) {
    Variables.insert(key, value);
}

void QSimpleVarExp::clear() {
    Variables.clear();
}

static QString dfs(QString s, const QRegularExpression &reg, const QHash<QString, QString> &vars) {
    QRegularExpressionMatch match;
    int index = 0;
    bool hasMatch = false;
    while ((index = s.indexOf(reg, index, &match)) != -1) {
        hasMatch = true;

        const auto &name = match.captured(1);
        QString val;
        auto it = vars.find(name);
        if (it == vars.end()) {
            val = name;
        } else {
            val = it.value();
        }
        s.replace(index, match.captured(0).size(), val);
    }
    if (!hasMatch) {
        return s;
    }
    return dfs(s, reg, vars);
}

QString QSimpleVarExp::parse(const QString &exp) const {
    return dfs(exp, QRegularExpression(Pattern), Variables);
}

QHash<QString, QString> QSimpleVarExp::SystemValues() {
    return {
        {"DESKTOP", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)},
        {"DOCUMENTS", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)},
        {"APPLICATIONS", QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)},
        {"HOME", QStandardPaths::writableLocation(QStandardPaths::HomeLocation)},
        {"APPDATA", QsSys::appDataPath()},
        {"TEMP", QDir::tempPath()},
        {"ROOT", QDir::rootPath()},
        {"APPPATH", qApp->applicationDirPath()},
        {"APPNAME", qAppName()},
    };
}
