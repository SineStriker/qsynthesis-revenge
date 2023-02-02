#ifndef QSCOREDECORATOR_P_H
#define QSCOREDECORATOR_P_H

#include <QTranslator>
#include <QSet>
#include <QSharedPointer>

#include "../QsCoreDecorator.h"

/**
 * @brief Locale related
 *
 */
struct LocaleSubscriber {
    QStringList keys;
    std::function<void()> updater;
};

struct QSBASE_API LocaleData {
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

struct  LocalePlaceholder {
    QSet<LocaleSubscriber *> subscribers;
    QSharedPointer<LocaleData> data;
};

class QSBASE_API QsCoreDecoratorPrivate {
    Q_DECLARE_PUBLIC(QsCoreDecorator)
public:
    QsCoreDecoratorPrivate();
    virtual ~QsCoreDecoratorPrivate();

    void init();

    QsCoreDecorator *q_ptr;

    // Locale related
    QString loc;
    QHash<QString, int> localeNames;
    QHash<QString, LocalePlaceholder *> localeConfigs;
    QHash<QObject *, LocaleSubscriber *> localeSubscribers;
};

#endif // QSCOREDECORATOR_P_H
