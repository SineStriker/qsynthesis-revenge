#ifndef CDECORATORPRIVATE_H
#define CDECORATORPRIVATE_H

#include "../CDecorator.h"
#include "private/QsCoreDecorator_p.h"

#include "ThemeTemplate.h"

#include <set>

#include <QHash>
#include <QSet>
#include <QTranslator>

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


class CDecoratorPrivate : public QsCoreDecoratorPrivate {
    Q_DECLARE_PUBLIC(CDecorator)
public:
    CDecoratorPrivate();
    virtual ~CDecoratorPrivate();

    void init();

    // Theme related
    QString theme;
    QHash<QString, int> themeNames;                                // themeKey - refCount
    QHash<QString, ThemeConfigPack> themeConfigs;                  // configKey
    QHash<QString, ThemePlaceholder *> themeTemplates;             // templateKey
    QHash<QSet<QString>, ThemeSubscriber *> themeSubscriberGroups; // templateKeys
    QHash<QWidget *, ThemeSubscriber *> themeSubscribers;          // w
};

#endif // CDECORATORPRIVATE_H
