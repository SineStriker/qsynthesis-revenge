#ifndef CDECORATORPRIVATE_H
#define CDECORATORPRIVATE_H

#include "../CDecorator.h"

#include "ThemeSubscriber.h"
#include "ThemeTemplate.h"

#include <QHash>
#include <QSet>
#include <QTranslator>

struct LocaleData {
    // QM file paths
    QMap<int, QStringList> qmFiles;

    // Installed translators
    QList<QTranslator *> translators;

    // Subscribers
    QSet<QWidget *> subscribers;

    LocaleData();
    ~LocaleData();

    // Apply translation
    void install(int loc);

    // Remove translation
    void uninstall();
};

struct LocaleSubscriber {
    QStringList keys;
    std::function<void()> updater;
};

struct ThemeData {
    // Json config paths
    QMap<int, QStringList> configs;
};

class CDecoratorPrivate {
    Q_DECLARE_PUBLIC(CDecorator)
public:
    CDecoratorPrivate();
    virtual ~CDecoratorPrivate();

    void init();

    CDecorator *q_ptr;

    // Locale related
    int loc;
    QHash<QString, LocaleData> locales;
    QHash<QWidget *, LocaleSubscriber> localeSubscribers;

    // Theme related
    int theme;
    int themeMax;
    QHash<QString, ThemeData> themeConfigs;
    QHash<QString, QSharedPointer<ThemeTemplate>> themeTemplates;
    QHash<QWidget *, ThemeSubscriber *> themeSubscribers;
};

#endif // CDECORATORPRIVATE_H
