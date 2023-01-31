#ifndef CDECORATORPRIVATE_H
#define CDECORATORPRIVATE_H

#include "../CDecorator.h"

#include "ThemeTemplate.h"

#include <set>

#include <QHash>
#include <QSet>
#include <QTranslator>

/**
 * @brief Locale related
 *
 */
struct LocaleSubscriber {
    QStringList keys;
    std::function<void()> updater;
};

struct LocaleData {
    // QM file paths
    QMap<QLocale, QStringList> qmFiles;

    // Installed translators
    QList<QTranslator *> translators;

    LocaleData();
    ~LocaleData();

    // Apply translation
    void install(const QLocale &loc);

    void install(const QStringList &paths);

    // Remove translation
    void uninstall();
};

struct LocalePlaceholder {
    QSet<LocaleSubscriber *> subscribers;
    QSharedPointer<LocaleData> data;
};


/**
 * @brief Theme related
 *
 */
class ThemeGuard;
struct ThemePlaceholder;

struct ThemeSubscriber {
    bool dirty;

    QHash<QWidget *, ThemeGuard *> widgets;
    QHash<QString, ThemePlaceholder *> templates;

    ThemeSubscriber();
    void notifyAll();
};

struct ThemePlaceholder {
    QString ns;
    QSet<ThemeSubscriber *> subscribers;

    struct ConfigSet {
        QMap<int, QHash<QString, QSharedPointer<ThemeConfig>>>
            data; // priority - (config_key - config)
    };
    QHash<QString, ConfigSet> configs; // theme_key - config_set

    QSharedPointer<ThemeTemplate> data;

    // Incremental update
    QSet<QString> dirtyThemeKeys;
    QHash<QString, QString> stylesheetCaches;

    bool isEmpty() const;
    void invalidate();
};

struct ThemeConfigPack {
    QHash<QString, QSharedPointer<ThemeConfig>> data; // theme_key - config
};


class CDecoratorPrivate {
    Q_DECLARE_PUBLIC(CDecorator)
public:
    CDecoratorPrivate();
    virtual ~CDecoratorPrivate();

    void init();

    CDecorator *q_ptr;

    // Locale related
    QLocale loc;
    QHash<QLocale, LocalePlaceholder *> localeConfigs;
    QHash<QWidget *, LocaleSubscriber *> localeSubscribers;

    // Theme related
    QString theme;
    QHash<QString, ThemeConfigPack> themeConfigs;                  // configKey
    QHash<QString, ThemePlaceholder *> themeTemplates;             // templateKey
    QHash<QSet<QString>, ThemeSubscriber *> themeSubscriberGroups; // templateKeys
    QHash<QWidget *, ThemeSubscriber *> themeSubscribers;          // w
};

#endif // CDECORATORPRIVATE_H
