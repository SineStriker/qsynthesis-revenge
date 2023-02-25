#ifndef QMCOREDECORATOR_P_H
#define QMCOREDECORATOR_P_H

#include <QSet>
#include <QSharedPointer>
#include <QTranslator>

#include "../QMCoreDecorator.h"

/**
 * @brief Locale related
 *
 */
struct LocaleSubscriber {
    QStringList keys;
    std::function<void()> updater;
};

struct QMCORELIB_API LocaleData {
    // QM file paths
    QHash<QString, QStringList> qmFiles;

    // Installed translators
    QList<QTranslator *> translators;

    LocaleData();
    ~LocaleData();

    // Apply translation
    void install(const QString &loc);

    void install(const QStringList &paths);

    // Remove translation
    void uninstall();
};

struct LocalePlaceholder {
    QSet<LocaleSubscriber *> subscribers;
    QSharedPointer<LocaleData> data;
};

class QMCORELIB_API QMCoreDecoratorPrivate {
    Q_DECLARE_PUBLIC(QMCoreDecorator)
public:
    QMCoreDecoratorPrivate();
    virtual ~QMCoreDecoratorPrivate();

    void init();

    QMCoreDecorator *q_ptr;

    // Theme related
    QString theme;
    QHash<QString, int> themeNames; // themeKey - refCount

    // Locale related
    QString loc;
    QHash<QString, int> localeNames;
    QHash<QString, LocalePlaceholder *> localeConfigs;
    QHash<QObject *, LocaleSubscriber *> localeSubscribers;
};

#endif // QMCOREDECORATOR_P_H
