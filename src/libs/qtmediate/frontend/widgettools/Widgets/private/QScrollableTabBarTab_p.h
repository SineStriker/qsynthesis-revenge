#ifndef QSCROLLABLETABBARTABPRIVATE_H
#define QSCROLLABLETABBARTABPRIVATE_H

#include <QAbstractButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>

class QScrollableTabBarTab;

class QScrollableTabBarTabPrivate {
    Q_DECLARE_PUBLIC(QScrollableTabBarTab)
public:
    QScrollableTabBarTabPrivate();
    virtual ~QScrollableTabBarTabPrivate();

    void init();

    void updateIconAndText();

    QScrollableTabBarTab *q_ptr;

    QHBoxLayout *layout;

    QIcon icon;
    QSize iconSize;
    QMargins iconMargins;

    QString text;
    QMargins textMargins;

    QSpacerItem *iconTextItem;
    QAbstractButton *closeButton;

    QVariant data;
};

#endif // QSCROLLABLETABBARTABPRIVATE_H
