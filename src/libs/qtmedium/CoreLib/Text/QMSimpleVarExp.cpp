#include "QMSimpleVarExp.h"

#include "QMSystem.h"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

static const char Slash = '/';

static const char DefaultPattern[] = R"(\$\{(\w+)\})";

QMSimpleVarExp::QMSimpleVarExp() : QMSimpleVarExp(DefaultPattern) {
}

QMSimpleVarExp::QMSimpleVarExp(const QString &pattern) : Pattern(pattern) {
}

QMSimpleVarExp::~QMSimpleVarExp() {
}

void QMSimpleVarExp::addJsonObject(const QJsonObject &obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        if (it->isString()) {
            Variables.insert(it.key(), it->toString());
        }
    }
}

void QMSimpleVarExp::addVariantMap(const QVariantMap &map) {
    for (auto it = map.begin(); it != map.end(); ++it) {
        if (it->type() == QVariant::String) {
            Variables.insert(it.key(), it->toString());
        }
    }
}

void QMSimpleVarExp::addMap(const QMap<QString, QString> &map) {
    for (auto it = map.begin(); it != map.end(); ++it) {
        Variables.insert(it.key(), it.value());
    }
}

void QMSimpleVarExp::addHash(const QHash<QString, QString> &hash) {
    for (auto it = hash.begin(); it != hash.end(); ++it) {
        Variables.insert(it.key(), it.value());
    }
}

void QMSimpleVarExp::add(const QString &key, const QString &value) {
    Variables.insert(key, value);
}

void QMSimpleVarExp::clear() {
    Variables.clear();
}

static QString dfs(QString s, const QRegularExpression &reg, const QHash<QString, QString> &vars,
                   bool recursive = true) {
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

QString QMSimpleVarExp::parse(const QString &exp) const {
    return dfs(exp, QRegularExpression(Pattern), Variables);
}

QHash<QString, QString> QMSimpleVarExp::SystemValues() {
    return {
        {"DESKTOP",      QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)     },
        {"DOCUMENTS",    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)   },
        {"APPLICATIONS", QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)},
        {"HOME",         QStandardPaths::writableLocation(QStandardPaths::HomeLocation)        },
        {"APPDATA",      QMFs::appDataPath()                                                   },
        {"TEMP",         QDir::tempPath()                                                      },
        {"ROOT",         QDir::rootPath()                                                      },
        {"APPPATH",      qApp->applicationDirPath()                                            },
        {"APPNAME",      qApp->applicationName()                                               },
    };
}

QString QMSimpleVarExp::EvaluateVariables(const QString &s, const QHash<QString, QString> &dict, const QString &pattern,
                                          bool recursive) {
    return dfs(s, QRegularExpression(pattern.isEmpty() ? DefaultPattern : pattern), dict, recursive);
}
