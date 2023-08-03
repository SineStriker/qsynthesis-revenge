#include "QMDecoratorV2.h"
#include "private/QMDecoratorV2_p.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QWindow>

#include <QMChronSet.h>
#include <QMSimpleVarExp.h>
#include <QMSystem.h>

#include "private/QMGuiAppExtension_p.h"

ThemeGuardV2::ThemeGuardV2(QWidget *w, QMDecoratorV2Private *parent)
    : QObject(parent), w(w), d(parent), winHandle(nullptr) {
    needUpdate = false;
    w->installEventFilter(this);
}

ThemeGuardV2::~ThemeGuardV2() {
}

void ThemeGuardV2::updateScreen() {
    if (!screen) {
        switchScreen(w->screen());
        return;
    }

    if (!winHandle) {
        needUpdate = true;
        return;
    }

    auto getStyleSheet = [&](const QString &theme) {
        QString allStylesheets;

        QString base = theme;
        QMChronSet<QString> bases{base};
        while (!(base = d->variables.value(base).value("_base")).isEmpty() &&
               !bases.contains(base) // Avoid recursively referenced
        ) {
            bases.prepend(base);
        }

        for (const auto &curTheme : qAsConst(bases)) {
            auto map = d->stylesheetCaches.value(curTheme, {});
            if (map.isEmpty()) {
                continue;
            }

            // Go through themes
            for (const auto &id : qAsConst(ids)) {
                for (const auto &key : d->nsMappings.value(id, {})) {
                    auto stylesheet = map.value(key, {});
                    if (stylesheet.isEmpty()) {
                        continue;
                    }

                    // Evaluate variables
                    stylesheet = QMSimpleVarExp::EvaluateVariables(stylesheet, d->variables.value(curTheme, {}),
                                                                   R"(\$\{([^\}]+)\})");

                    stylesheet = QMDecoratorV2Private::replaceSizes(
                        stylesheet, screen->logicalDotsPerInch() / QMOs::unitDpi(), true);

                    // Zoom
                    allStylesheets += stylesheet + "\n\n";
                }
            }
        }

        return allStylesheets;
    };

    QString stylesheets = getStyleSheet("_common");
    if (!stylesheets.isEmpty()) {
        stylesheets += "\n\n";
    }
    stylesheets += getStyleSheet(d->currentTheme);

    if (stylesheets.isEmpty())
        return;

    // qDebug().noquote() << stylesheets;

    w->setStyleSheet(stylesheets);
}

void ThemeGuardV2::switchScreen(QScreen *screen) {
    if (!screen || this->screen == screen)
        return;

    if (this->screen) {
        disconnect(this->screen.data(), &QScreen::logicalDotsPerInchChanged, this,
                   &ThemeGuardV2::_q_logicalRatioChanged);
    }
    this->screen = screen;
    connect(screen, &QScreen::logicalDotsPerInchChanged, this, &ThemeGuardV2::_q_logicalRatioChanged);

    updateScreen();
}

bool ThemeGuardV2::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::PolishRequest: {
            break;
        }
        case QEvent::Show: {
            if (!winHandle) {
                winHandle = w->window()->windowHandle();
                connect(winHandle, &QWindow::screenChanged, this, &ThemeGuardV2::switchScreen);

                if (needUpdate) {
                    updateScreen();
                }
            }
            break;
        }
        default:
            break;
    }
    return QObject::eventFilter(obj, event);
}

void ThemeGuardV2::_q_logicalRatioChanged(double dpi) {
    Q_UNUSED(dpi)

    updateScreen();
}

QMDecoratorV2Private::QMDecoratorV2Private() {
}

QMDecoratorV2Private::~QMDecoratorV2Private() {
}

void QMDecoratorV2Private::init() {
    currentTheme = "Visual Studio Code - Dark";
}

struct QssItem {
    double ratio;
    QString content;
    QString fileName;

    friend QDebug operator<<(QDebug debug, const QssItem &item) {
        debug << "QssItem(" << item.ratio << ", " << (item.fileName.isEmpty() ? item.content : item.fileName) << ")";
        return debug;
    }
};

template <class T>
static T parsePlatform(const QJsonValue &val, bool(predicate)(const QJsonValue &), T(convert)(const QJsonValue &),
                       const T &defaultValue = T{}) {
    QStringList platformKeys{
#ifdef Q_OS_WINDOWS
        "win", "win32", "windows"
#elif defined(Q_OS_LINUX)
        "linux"
#else
        "mac", "macos", "macosx"
#endif
    };

    if (predicate(val)) {
        return convert(val);
    }

    auto obj = val.toObject();
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        if (platformKeys.contains(it.key(), Qt::CaseInsensitive)) {
            if (predicate(it.value())) {
                return convert(it.value());
            }
            return defaultValue;
        }
    }
    return defaultValue;
};

static double parsePlatformDouble(const QJsonValue &val, double defaultValue = 0) {
    return parsePlatform<double>(
        val, [](const QJsonValue &val) { return val.isDouble(); }, [](const QJsonValue &val) { return val.toDouble(); },
        defaultValue);
}

static QString parsePlatformString(const QJsonValue &val, const QString &defaultValue = {}) {
    return parsePlatform<QString>(
        val, [](const QJsonValue &val) { return val.isString(); }, [](const QJsonValue &val) { return val.toString(); },
        defaultValue);
}

void QMDecoratorV2Private::scanForThemes() const {
    stylesheetCaches.clear();
    nsMappings.clear();
    variables.clear();

    QFileInfoList searchFiles;
    for (const auto &path : qAsConst(themePaths)) {
        QStringList searchPaths = {path};
        while (!searchPaths.isEmpty()) {
            const QDir dir(searchPaths.takeFirst());
            const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
            foreach (const QFileInfo &file, files) {
                if (!file.completeSuffix().compare("res.json", Qt::CaseInsensitive)) {
                    searchFiles.append(file);
                }
            }
            const QFileInfoList dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            foreach (const QFileInfo &subdir, dirs)
                searchPaths << subdir.absoluteFilePath();
        }
    }

    // theme - [ namespace - [ priority - items] ]
    QMap<QString, QMap<QString, QMap<double, QList<QssItem>>>> tmp;

    QHash<QString, QHash<QString, double>> variablesPriorities;
    for (const auto &file : searchFiles) {
        QFile f(file.absoluteFilePath());
        if (!f.open(QIODevice::ReadOnly)) {
            continue;
        }

        QByteArray data(f.readAll());
        f.close();

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            continue;
        }

        double ratio = 1;
        double priority = 1;

        auto objDoc = doc.object();

        QJsonValue value;

        // Get default values
        value = objDoc.value("config");
        if (!value.isUndefined() && value.isObject()) {
            auto obj = value.toObject();
            value = obj.value("ratio");
            if (!value.isUndefined()) {
                auto _tmp = parsePlatformDouble(value);
                if (_tmp > 0)
                    ratio = _tmp;
            }

            value = obj.value("priority");
            if (!value.isUndefined()) {
                auto _tmp = parsePlatformDouble(value, -1);
                if (_tmp >= 0)
                    priority = _tmp;
            }
        }

        value = objDoc.value("variables");
        if (!value.isUndefined() && value.isObject()) {
            auto obj = value.toObject();
            for (auto it0 = obj.begin(); it0 != obj.end(); ++it0) {
                if (!it0->isObject()) {
                    continue;
                }
                auto themeObj = it0->toObject();
                if (themeObj.isEmpty()) {
                    continue;
                }

                const auto &themeKey = it0.key();
                auto &priorityMap = variablesPriorities[themeKey];
                auto &variableMap = variables[themeKey];

                for (auto it = themeObj.begin(); it != themeObj.end(); ++it) {
                    const auto &key = it.key();
                    QString val;
                    double _priority = priority;

                    if (it->isObject()) {
                        auto varObj = it->toObject();
                        value = varObj.value("priority");
                        if (value.isDouble()) {
                            priority = value.toDouble();
                        }
                        value = varObj.value("value");
                        if (value.isString()) {
                            val = value.toString();
                        }
                    } else if (it->isString()) {
                        val = it->toString();
                    }

                    auto it2 = priorityMap.find(key);
                    if (it2 == priorityMap.end()) {
                        priorityMap.insert(key, _priority);
                        variableMap[key] = val;
                    } else if (it2.value() > _priority) {
                        it2.value() = _priority;
                        variableMap[key] = val;
                    }
                }
            }
        }

        auto parseStyleObject = [&](QMap<QString, QMap<double, QList<QssItem>>> &map, const QString &key,
                                    const QJsonObject &obj) {
            QssItem item{ratio, {}, {}};
            QJsonValue value;

            value = obj.value("file");
            if (!value.isUndefined()) {
                QString fileName = parsePlatformString(value);
                if (!fileName.isEmpty()) {
                    if (QDir::isRelativePath(fileName)) {
                        fileName = file.absolutePath() + "/" + fileName;
                    }
                    item.fileName = fileName;
                    goto out;
                }
            }

            value = obj.value("content");
            if (!value.isUndefined()) {
                QString content = parsePlatformString(value);
                if (!content.isEmpty()) {
                    item.content = content;
                    goto out;
                }
            }

            return;

        out:
            value = obj.value("ratio");
            if (!value.isUndefined()) {
                auto _tmp = parsePlatformDouble(value);
                if (_tmp > 0)
                    item.ratio = _tmp;
            }

            auto _priority = priority;

            value = obj.value("priority");
            if (!value.isUndefined()) {
                auto _tmp = parsePlatformDouble(value, -1);
                if (_tmp >= 0)
                    _priority = _tmp;
            }

            map[key][_priority].append(item);
        };

        // Get namespaces
        value = objDoc.value("widgets");
        if (!value.isUndefined() && value.isObject()) {
            auto obj = value.toObject();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                QStringList keys;
                if (it->isArray()) {
                    for (const auto &item : it->toArray()) {
                        if (item.isString())
                            keys.append(item.toString());
                    }
                } else if (it->isString()) {
                    keys.append(it->toString());
                }
                nsMappings[it.key()].append(keys);
            }
        }

        value = objDoc.value("stylesheets");
        if (!value.isUndefined() && value.isObject()) {
            auto obj = value.toObject();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                if (!it->isObject()) {
                    continue;
                }

                const QString &themeKey = it.key();
                auto themeObj = it->toObject();

                // namespace - [ priority - items ]
                QMap<QString, QMap<double, QList<QssItem>>> map;
                for (auto it1 = themeObj.begin(); it1 != themeObj.end(); ++it1) {
                    if (it1->isArray()) {
                        for (const auto &item : it1->toArray()) {
                            if (item.isObject()) {
                                parseStyleObject(map, it1.key(), item.toObject());
                            }
                        }
                    } else if (it1->isObject()) {
                        parseStyleObject(map, it1.key(), it1->toObject());
                    }
                }

                if (map.isEmpty()) {
                    continue;
                }

                auto &themeMap = tmp[themeKey];
                for (auto it1 = map.begin(); it1 != map.end(); ++it1) {
                    const auto &itemMap = it1.value();
                    if (itemMap.isEmpty()) {
                        continue;
                    }

                    auto &nsMap = themeMap[it1.key()];
                    for (auto it2 = itemMap.begin(); it2 != itemMap.end(); ++it2) {
                        if (it2->isEmpty()) {
                            continue;
                        }
                        nsMap[it2.key()].append(it2.value());
                    }
                }
            }
        }
    }

    // Get stylesheet contents
    auto readStyleSheets = [](const QMap<double, QList<QssItem>> &map) {
        QString res;
        for (auto it = --map.end(); it != --map.begin(); it--) {
            for (const auto &item : qAsConst(it.value())) {
                QString content;
                if (!item.fileName.isEmpty()) {
                    QFile f(item.fileName);
                    if (!f.open(QIODevice::ReadOnly)) {
                        continue;
                    }
                    content = QString::fromUtf8(f.readAll());

                    // Replace relative paths
                    QFileInfo info(item.fileName);
                    content.replace(QRegularExpression(R"(@[/\\])"), info.absolutePath() + "/");
                    f.close();
                } else {
                    content = item.content;
                }

                if (content.isEmpty()) {
                    continue;
                }

                res.append(QMDecoratorV2::evaluateStyleSheet(content, item.ratio) + "\n\n");
            }
        }

        return res;
    };

    for (auto it = tmp.begin(); it != tmp.end(); ++it) {
        const auto &themeKey = it.key();
        const auto &themeMap = it.value();

        QMap<QString, QString> styleMap;
        for (auto it2 = themeMap.begin(); it2 != themeMap.end(); ++it2) {
            styleMap.insert(it2.key(), readStyleSheets(it2.value()));
        }

        if (styleMap.isEmpty()) {
            continue;
        }

        stylesheetCaches.insert(themeKey, styleMap);
    }

    themeFilesDirty = false;
}

QString QMDecoratorV2Private::replaceSizes(const QString &stylesheet, double ratio, bool rounding) {
    QRegularExpression re(R"([0-9]+(\.[0-9]+|)px)");
    QRegularExpressionMatch match;
    int index = 0;

    QString Content = stylesheet;

    while ((index = Content.indexOf(re, index, &match)) != -1) {
        QString MatchString = match.captured();

        double size = MatchString.midRef(0, MatchString.size() - 2).toDouble();
        size *= ratio;
        QString ValueString = (rounding ? QString::number(int(size)) : QString::number(size)) + "px";

        Content.replace(index, MatchString.size(), ValueString);
        index += ValueString.size();
    }

    return Content;
}

QString QMDecoratorV2Private::replaceCustomKeyWithQProperty(const QString &stylesheet) {
    // Replace "--key: value;" with "qproperty-key: value;"
    // Replace "---key: value;" with "key: value;"
    QRegularExpression re(R"((\{|;|^)\s*(--|---)\w(\w|-)*:)",
                          QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match;
    int index = 0;

    QString Content = stylesheet;
    while ((index = Content.indexOf(re, index, &match)) != -1) {
        QString MatchString = match.captured();
        QString ValueString = MatchString;

        int capturedIndex = match.capturedStart(2) - index;
        int capturedLen = match.capturedLength(2);
        ValueString.replace(capturedIndex, capturedLen, capturedLen == 2 ? "qproperty-" : "");

        Content.replace(index, MatchString.size(), ValueString);
        index += ValueString.size();
    }

    return Content;
}

QString QMDecoratorV2Private::replaceCssGrammars(const QString &stylesheet) {
    QString Content = stylesheet;

    // Replace ":not(:xxx)" with ":!xxx"
    {
        QRegularExpression re(R"(:not\(\s*:([^)]+)\s*\))",
                              QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
        QRegularExpressionMatch match;
        int index = 0;

        while ((index = Content.indexOf(re, index, &match)) != -1) {
            QString MatchString = match.captured();
            QString ValueString = ":!" + match.captured(1).trimmed();

            Content.replace(index, MatchString.size(), ValueString);
            index += ValueString.size();
        }
    }

    // Replace "svg(...);" to "url(\"..., .svgx\");"
    {
        QRegularExpression re(R"(svg\((.*?)\)(;|\s*\}))",
                              QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
        Content.replace(re, R"(url("\1, .svgx")\2)");
    }

    return Content;
}

QString QMDecoratorV2Private::removeAllComments(QString data) {
    QRegularExpression reg(R"(\/\*(.*?)\*\/)",
                           QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch match;
    int index = 0;
    while ((index = data.indexOf(reg, index, &match)) != -1) {
        data.remove(index, match.captured(0).size());
    }
    return data;
}

void QMDecoratorV2Private::_q_themeSubscriberDestroyed() {
    auto it = themeSubscribers.find(reinterpret_cast<QWidget *>(sender()));
    if (it == themeSubscribers.end()) {
        return;
    }
    delete it.value();
    themeSubscribers.erase(it);
}

QMDecoratorV2::QMDecoratorV2(QObject *parent) : QMDecoratorV2(*new QMDecoratorV2Private(), parent) {
}

QMDecoratorV2::~QMDecoratorV2() {
}

QString QMDecoratorV2::evaluateStyleSheet(const QString &stylesheet, double ratio) {
    QString content = stylesheet;

    // Remove comments
    content = QMDecoratorV2Private::removeAllComments(content);

    // Replace custom keys
    content = QMDecoratorV2Private::replaceCustomKeyWithQProperty(content);

    // Replace CSS grammars
    content = QMDecoratorV2Private::replaceCssGrammars(content);

    if (ratio != 1 && ratio > 0) {
        content = QMDecoratorV2Private::replaceSizes(content, ratio, false);
    }

    return content;
}

void QMDecoratorV2::addThemePath(const QString &path) {
    Q_D(QMDecoratorV2);

    if (path.isEmpty())
        return;

    QString canonicalPath = QDir(path).canonicalPath();
    if (canonicalPath.isEmpty())
        return;

    if (d->themePaths.contains(path))
        return;

    d->themePaths.insert(path);
    d->themeFilesDirty = true;
}

void QMDecoratorV2::removeThemePath(const QString &path) {
    Q_D(QMDecoratorV2);

    if (path.isEmpty())
        return;

    QString canonicalPath = QDir(path).canonicalPath();
    if (canonicalPath.isEmpty())
        return;

    auto it = d->themePaths.find(path);
    if (it == d->themePaths.end())
        return;

    d->themePaths.erase(it);
    d->themeFilesDirty = true;
}

QStringList QMDecoratorV2::themes() const {
    Q_D(const QMDecoratorV2);
    if (d->themeFilesDirty) {
        d->scanForThemes();
    }
    QStringList res = d->stylesheetCaches.keys();
    res.removeOne("_common");
    return res;
}

QString QMDecoratorV2::theme() const {
    Q_D(const QMDecoratorV2);
    return d->currentTheme;
}

void QMDecoratorV2::setTheme(const QString &theme) {
    Q_D(QMDecoratorV2);

    if (d->themeFilesDirty) {
        d->scanForThemes();
    } else if (d->currentTheme == theme) {
        return;
    }

    d->currentTheme = theme;
    QMGuiAppExtensionPrivate::globalImageCacheSerialNum++;

    for (const auto &item : qAsConst(d->themeSubscribers)) {
        item->updateScreen();
    }
    emit themeChanged(theme);
}

void QMDecoratorV2::refreshTheme() {
    Q_D(QMDecoratorV2);
    setTheme(d->currentTheme);
}

QString QMDecoratorV2::themeVariable(const QString &key) const {
    Q_D(const QMDecoratorV2);
    return d->variables.value(d->currentTheme, {}).value(key);
}

void QMDecoratorV2::installTheme(QWidget *w, const QString &id) {
    Q_D(QMDecoratorV2);

    if (d->themeFilesDirty) {
        refreshTheme();
    }

    auto it = d->themeSubscribers.find(w);
    if (it == d->themeSubscribers.end()) {
        connect(w, &QWidget::destroyed, d, &QMDecoratorV2Private::_q_themeSubscriberDestroyed);
        it = d->themeSubscribers.insert(w, new ThemeGuardV2(w, d));
    }
    auto &tg = *it.value();
    tg.ids += id;
    tg.updateScreen();
}

QMDecoratorV2::QMDecoratorV2(QMDecoratorV2Private &d, QObject *parent) : QMGuiDecoratorV2(d, parent) {
    d.init();
}