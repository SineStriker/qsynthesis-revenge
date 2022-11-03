#ifndef ANATIVETITLEBARPRIVATE_H
#define ANATIVETITLEBARPRIVATE_H

#include <QHBoxLayout>

#include "../ANativeTitleBar.h"
#include "../ANativeTitleBarLayout.h"

class ANativeTitleBarPrivate {
    Q_DECLARE_PUBLIC(ANativeTitleBar)
public:
    ANativeTitleBarPrivate();
    virtual ~ANativeTitleBarPrivate();

    void init();

    ANativeTitleBar *q_ptr;

    ANativeTitleBarLayout *layout;
    QWidget *w;

    bool autoTitle;
    bool autoIcon;

    QMenuBar *menuBar() const;
    QLabel *titleLabel() const;
    QAbstractButton *iconButton() const;
    QAbstractButton *minButton() const;
    QAbstractButton *maxButton() const;
    QAbstractButton *closeButton() const;

    void setMenuBar(QMenuBar *bar);
    void setTitleLabel(QLabel *label);
    void setIconButton(QAbstractButton *btn);
    void setMinButton(QAbstractButton *btn);
    void setMaxButton(QAbstractButton *btn);
    void setCloseButton(QAbstractButton *btn);

    QMenuBar *takeMenuBar();
    QLabel *takeTitleLabel();
    QAbstractButton *takeIconButton();
    QAbstractButton *takeMinButton();
    QAbstractButton *takeMaxButton();
    QAbstractButton *takeCloseButton();
};

#endif // ANATIVETITLEBARPRIVATE_H
