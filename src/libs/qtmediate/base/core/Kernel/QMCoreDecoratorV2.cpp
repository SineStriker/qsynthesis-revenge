#include "QMCoreDecoratorV2.h"
#include "private/QMCoreDecoratorV2_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QRegularExpression>

Q_SINGLETON_DECLARE(QMCoreDecoratorV2)

QMCoreDecoratorV2Private::QMCoreDecoratorV2Private() {
    qmFilesDirty = false;
}

QMCoreDecoratorV2Private::~QMCoreDecoratorV2Private() {
}

void QMCoreDecoratorV2Private::init() {
    currentLocale = QLocale::system().name();
}

QMCoreDecoratorV2::QMCoreDecoratorV2(QObject *parent) : QMCoreDecoratorV2(*new QMCoreDecoratorV2Private(), parent) {
}

QMCoreDecoratorV2::~QMCoreDecoratorV2() {
    destruct();
}

static QMap<QString, QStringList> scanTranslation_helper(const QString &path) {
    QMap<QString, QStringList> res;

    QFileInfoList searchFiles;
    QStringList searchPaths = {path};
    while (!searchPaths.isEmpty()) {
        const QDir dir(searchPaths.takeFirst());
        const QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoSymLinks);
        foreach (const QFileInfo &file, files) {
            if (!file.suffix().compare("qm", Qt::CaseInsensitive)) {
                searchFiles.append(file);
            }
        }
        const QFileInfoList dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (const QFileInfo &subdir, dirs)
            searchPaths << subdir.absoluteFilePath();
    }

    QRegularExpression reg(R"((\w+?)_(\w{2})(_\w+|))");
    for (const auto &file : qAsConst(searchFiles)) {
        auto match = reg.match(file.fileName());
        if (!match.hasMatch()) {
            continue;
        }

        res[match.captured(2) + match.captured(3)].append(file.absoluteFilePath());
    }
    return res;
}

static QList<QTranslator *> installTranslation_helper(const QStringList &paths) {
    QList<QTranslator *> res;
    for (const auto &file : qAsConst(paths)) {
        auto t = new QTranslator(qApp);
        if (!t->load(file)) {
            delete t;
            continue;
        }
        qApp->installTranslator(t);
        res.append(t);
    }
    return res;
}

void QMCoreDecoratorV2Private::scanTranslations() const {
    qmFiles.clear();

    for (const auto &path : qAsConst(translationPaths)) {
        insertTranslationFiles_helper(scanTranslation_helper(path));
    }

    qmFilesDirty = false;
}

void QMCoreDecoratorV2Private::insertTranslationFiles_helper(const QMap<QString, QStringList> &map) const {
    for (auto it = map.begin(); it != map.end(); ++it) {
        if (it->isEmpty()) {
            continue;
        }
        qmFiles[it.key()].append(it.value());
    }
}

void QMCoreDecoratorV2Private::_q_localeSubscriberDestroyed() {
    localeSubscribers.remove(sender());
}

void QMCoreDecoratorV2::addTranslationPath(const QString &path) {
    Q_D(QMCoreDecoratorV2);

    if (path.isEmpty())
        return;

    QString canonicalPath = QDir(path).canonicalPath();
    if (canonicalPath.isEmpty())
        return;

    if (d->translationPaths.contains(path))
        return;

    d->translationPaths.insert(path);

    // d->qmFilesDirty = true;
    if (d->qmFilesDirty) {
        refreshLocale();
        return;
    }

    // Support incremental update when adding path
    auto map = scanTranslation_helper(path);
    d->insertTranslationFiles_helper(map);

    // Install new translators
    auto it = map.find(d->currentLocale);
    if (it == map.end()) {
        return;
    }
    auto translators = installTranslation_helper(it.value());
    d->translators.append(translators);

    for (const auto &item : qAsConst(d->localeSubscribers))
        for (const auto &updater : qAsConst(item))
            updater();
}

void QMCoreDecoratorV2::removeTranslationPath(const QString &path) {
    Q_D(QMCoreDecoratorV2);

    if (path.isEmpty())
        return;

    QString canonicalPath = QDir(path).canonicalPath();
    if (canonicalPath.isEmpty())
        return;

    auto it = d->translationPaths.find(path);
    if (it == d->translationPaths.end())
        return;

    d->translationPaths.erase(it);
    d->qmFilesDirty = true;
}

QStringList QMCoreDecoratorV2::locales() const {
    Q_D(const QMCoreDecoratorV2);
    if (d->qmFilesDirty) {
        d->scanTranslations();
    }

    return d->qmFiles.keys();
}

QString QMCoreDecoratorV2::locale() const {
    Q_D(const QMCoreDecoratorV2);
    return d->currentLocale;
}

void QMCoreDecoratorV2::setLocale(const QString &locale) {
    Q_D(QMCoreDecoratorV2);

    if (d->qmFilesDirty) {
        d->scanTranslations();
    } else if (d->currentLocale == locale) {
        return;
    }

    // Remove original translators
    qDeleteAll(d->translators);
    d->translators.clear();

    // Set new locale
    d->currentLocale = locale;

    // Install new translators
    auto it = d->qmFiles.find(locale);
    if (it != d->qmFiles.end()) {
        auto translators = installTranslation_helper(it.value());
        d->translators.append(translators);
    }

    for (const auto &item : qAsConst(d->localeSubscribers))
        for (const auto &updater : qAsConst(item))
            updater();

    emit localeChanged(locale);
}

void QMCoreDecoratorV2::refreshLocale() {
    Q_D(QMCoreDecoratorV2);
    setLocale(d->currentLocale);
}

void QMCoreDecoratorV2::installLocale(QObject *o, const std::function<void()> &updater) {
    Q_D(QMCoreDecoratorV2);

    if (d->qmFilesDirty) {
        refreshLocale();
    }

    auto it = d->localeSubscribers.find(o);
    if (it == d->localeSubscribers.end()) {
        connect(o, &QObject::destroyed, d, &QMCoreDecoratorV2Private::_q_localeSubscriberDestroyed);
        it = d->localeSubscribers.insert(o, {});
    }
    it->append(updater);
    updater();
}

QMCoreDecoratorV2::QMCoreDecoratorV2(QMCoreDecoratorV2Private &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();
}
