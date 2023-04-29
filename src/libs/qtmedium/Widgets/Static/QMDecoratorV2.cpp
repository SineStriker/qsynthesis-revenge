#include "QMDecoratorV2.h"
#include "private/QMDecoratorV2_p.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

#include <QWindow>

#include <QMSystem.h>

ThemeGuardV2::ThemeGuardV2(QWidget *w, QMDecoratorV2Private *parent)
    : QObject(parent), w(w), d(parent), winHandle(w->windowHandle()) {
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
        auto map = d->stylesheetCaches.value(theme, {});
        if (map.isEmpty()) {
            return QString();
        }

        QString allStylesheets;
        for (const auto &id : qAsConst(ids)) {
            for (const auto &key : d->nsMappings.value(id, {})) {
                auto stylesheet = map.value(key, {});
                if (stylesheet.isEmpty()) {
                    continue;
                }
                allStylesheets += QMDecoratorV2Private::replaceSizes(
                                      stylesheet, screen->logicalDotsPerInch() / QMOs::unitDpi(), true) +
                                  "\n\n";
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

    // Delay setting stylesheets
    QTimer::singleShot(0, this, &ThemeGuardV2::updateScreen);
}

QMDecoratorV2Private::QMDecoratorV2Private() {
}

QMDecoratorV2Private::~QMDecoratorV2Private() {
}

void QMDecoratorV2Private::init() {
    currentTheme = "Zhibin - Dark";
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

        auto parseStyleObject = [&](QMap<QString, QMap<double, QList<QssItem>>> &map, const QString &key,
                                    const QJsonObject &obj) {
            QssItem item{ratio, {}, {}};

            auto it = obj.find("file");
            if (it != obj.end()) {
                QString fileName = parsePlatformString(it.value());
                if (!fileName.isEmpty()) {
                    if (QDir::isRelativePath(fileName)) {
                        fileName = file.absolutePath() + "/" + fileName;
                    }
                    item.fileName = fileName;
                    goto out;
                }
            }

            it = obj.find("content");
            if (it != obj.end()) {
                QString content = parsePlatformString(it.value());
                if (!content.isEmpty()) {
                    item.content = content;
                    goto out;
                }
            }

            return;

        out:
            it = obj.find("ratio");
            if (it != obj.end()) {
                auto _tmp = parsePlatformDouble(it.value());
                if (_tmp > 0)
                    item.ratio = _tmp;
            }

            auto _priority = priority;

            it = obj.find("priority");
            if (it != obj.end()) {
                auto _tmp = parsePlatformDouble(it.value(), -1);
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

                // namespace - [ priority - items]
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
                    content.replace("@/", info.absolutePath() + "/");

                    f.close();
                } else {
                    content = item.content;
                }

                if (content.isEmpty()) {
                    continue;
                }

                content = QMDecoratorV2Private::removeAllComments(content);
                if (item.ratio != 1 && item.ratio > 0) {
                    content = QMDecoratorV2Private::replaceSizes(content, item.ratio, false);
                }

                res.append(content + "\n\n");
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
        QString ValueString;
        QString MatchString = match.captured();

        double size = MatchString.midRef(0, MatchString.size() - 2).toDouble();
        size *= ratio;
        ValueString = (rounding ? QString::number(int(size)) : QString::number(size)) + "px";

        Content.replace(index, MatchString.size(), ValueString);
        index += ValueString.size();
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
    return d->stylesheetCaches.keys();
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
    for (const auto &item : qAsConst(d->themeSubscribers)) {
        item->updateScreen();
    }

    emit themeChanged(theme);
}

void QMDecoratorV2::refreshTheme() {
    Q_D(QMDecoratorV2);
    setTheme(d->currentTheme);
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

QMDecoratorV2::QMDecoratorV2(QMDecoratorV2Private &d, QObject *parent) : QMCoreDecoratorV2(d, parent) {
    d.init();
}