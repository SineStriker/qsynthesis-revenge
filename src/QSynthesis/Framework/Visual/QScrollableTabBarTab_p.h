#ifndef QSCROLLABLETABBARTABPRIVATE_H
#define QSCROLLABLETABBARTABPRIVATE_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVariant>
#include <QtGlobal>

class QScrollableTabBarTab;

class QScrollableTabBarTabPrivate {
    Q_DECLARE_PUBLIC(QScrollableTabBarTab)
public:
    QScrollableTabBarTabPrivate();
    ~QScrollableTabBarTabPrivate();

    void init();

    QScrollableTabBarTab *q_ptr;

    QHBoxLayout *layout;

    QPushButton *iconButton;
    QLabel *textLabel;
    QPushButton *closeButton;

    QVariant data;
};

#endif // QSCROLLABLETABBARTABPRIVATE_H
