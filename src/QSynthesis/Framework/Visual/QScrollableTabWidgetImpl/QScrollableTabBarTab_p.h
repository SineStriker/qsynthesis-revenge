#ifndef QSCROLLABLETABBARTABPRIVATE_H
#define QSCROLLABLETABBARTABPRIVATE_H

#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>

#include "CPushButton.h"

class QScrollableTabBarTab;

class QScrollableTabBarTabPrivate {
    Q_DECLARE_PUBLIC(QScrollableTabBarTab)
public:
    QScrollableTabBarTabPrivate();
    ~QScrollableTabBarTabPrivate();

    void init();

    QScrollableTabBarTab *q_ptr;

    QHBoxLayout *layout;

    CPushButton *iconButton;
    QLabel *textLabel;
    CPushButton *closeButton;

    QVariant data;
};

#endif // QSCROLLABLETABBARTABPRIVATE_H
