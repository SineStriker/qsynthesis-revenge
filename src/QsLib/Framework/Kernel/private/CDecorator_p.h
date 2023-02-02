#ifndef CDECORATORPRIVATE_H
#define CDECORATORPRIVATE_H

#include "../CDecorator.h"
#include "private/QsCoreDecorator_p.h"

#include "ThemeTemplate.h"

#include <set>

#include <QHash>
#include <QSet>
#include <QTranslator>

#define ENABLE_PARTIAL_OVERRIDE 1

/**
 * @brief Theme related
 *
 */
class ThemeGuard;
struct ThemePlaceholder;

struct ScreenSet {
    bool dirty;
    QScreen *screen;
    QHash<QWidget *, ThemeGuard *> widgets;

    ScreenSet(QScreen *screen);
};

struct ThemeSubscriber {
    QStringList keySeq;
    QHash<QScreen *, ScreenSet *> screens;
    QHash<QWidget *, ThemeGuard *> widgets;
    QHash<QString, ThemePlaceholder *> templates;

    ThemeSubscriber();
    void notifyAll();

    void installScreen(ThemeGuard *tg);
    void uninstallScreen(ThemeGuard *tg);
    void switchScreen(ThemeGuard *tg);

    bool isEmpty() const;
    void addWidget(QWidget *w);
    void removeWidget(QWidget *w);
};

struct ThemePlaceholder {
    QString ns;
    QSet<ThemeSubscriber *> subscribers;

    struct ConfigSet {
        std::map<int, QHash<QString, QSharedPointer<ThemeConfig>>>
            data; // priority - (config_key - config)
    };
    QHash<QString, ConfigSet> configs; // theme_key - config_set

    QSharedPointer<ThemeTemplate> data;

    // Incremental update
    QHash<QScreen *, QString> stylesheetCaches;

    bool isEmpty() const;
    void invalidate();
    QString getAndCache(QScreen *screen);
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

    void screenChange_helper(QScreen *screen);

    // Theme related
    QString theme;
    QHash<QString, int> themeNames;                              // themeKey - refCount
    QHash<QString, ThemeConfigPack> themeConfigs;                // configKey
    QHash<QString, ThemePlaceholder *> themeTemplates;           // templateKey
    QHash<QStringList, ThemeSubscriber *> themeSubscriberGroups; // templateKeys
    QHash<QWidget *, ThemeSubscriber *> themeSubscribers;        // w

    static std::list<ThemeGuard *> subscriberUpdateQueue;

    static void subscriberUpdateEnqueue(ThemeGuard *tg);

    // Internal use
    static CDecoratorPrivate *self;
};

#endif // CDECORATORPRIVATE_H
