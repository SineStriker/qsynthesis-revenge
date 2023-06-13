#ifndef QMDECORATORV2PRIVATE_H
#define QMDECORATORV2PRIVATE_H

#include <QHash>
#include <QMap>
#include <QPointer>
#include <QSet>
#include <QTranslator>
#include <QWidget>

#include "../QMDecoratorV2.h"
#include "private/QMGuiDecoratorV2_p.h"

class ThemeGuardV2 : public QObject {
    Q_OBJECT
public:
    explicit ThemeGuardV2(QWidget *w, QMDecoratorV2Private *parent);
    ~ThemeGuardV2();

    QWidget *w;
    QWindow *winHandle;

    bool needUpdate;
    QPointer<QScreen> screen;

    // bool needUpdate;

    QStringList ids;
    QMDecoratorV2Private *d;

    void updateScreen();
    void switchScreen(QScreen *screen);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void _q_logicalRatioChanged(double dpi);
};

class QMDecoratorV2Private : public QMGuiDecoratorV2Private {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMDecoratorV2)
public:
    QMDecoratorV2Private();
    virtual ~QMDecoratorV2Private();

    void init();

    void scanForThemes() const;

    QSet<QString> themePaths;
    QHash<QWidget *, ThemeGuardV2 *> themeSubscribers;
    QString currentTheme;

    mutable bool themeFilesDirty;
    mutable QMap<QString, QMap<QString, QString>> stylesheetCaches; // themeKey - [ namespace - content ]
    mutable QHash<QString, QStringList> nsMappings;                 // widgetKey - namespaces
    mutable QHash<QString, QHash<QString, QString>> variables;      // themeKey - [ varKey - var ]

    static QString replaceSizes(const QString &stylesheet, double ratio, bool rounding);
    static QString replaceCustomKeyWithQProperty(const QString &stylesheet);
    static QString replaceCssGrammars(const QString &stylesheet);
    static QString removeAllComments(QString data);

private:
    void _q_themeSubscriberDestroyed();
};

#endif // QMDECORATORV2PRIVATE_H