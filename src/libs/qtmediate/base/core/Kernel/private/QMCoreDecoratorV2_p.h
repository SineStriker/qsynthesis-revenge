#ifndef QMCOREDECORATORV2PRIVATE_H
#define QMCOREDECORATORV2PRIVATE_H

#include "../QMCoreDecoratorV2.h"

#include <QHash>
#include <QMap>
#include <QSet>
#include <QTranslator>

class QMCORE_EXPORT QMCoreDecoratorV2Private : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMCoreDecoratorV2)
public:
    QMCoreDecoratorV2Private();
    virtual ~QMCoreDecoratorV2Private();

    void init();

    void scanTranslations() const;

    void insertTranslationFiles_helper(const QMap<QString, QStringList> &map) const;

    QMCoreDecoratorV2 *q_ptr;

    QSet<QString> translationPaths;
    QList<QTranslator *> translators;
    QString currentLocale;
    QHash<QObject *, QList<std::function<void()>>> localeSubscribers;

    mutable bool qmFilesDirty;
    mutable QMap<QString, QStringList> qmFiles;

private:
    void _q_localeSubscriberDestroyed();
};

#endif // QMCOREDECORATORV2PRIVATE_H