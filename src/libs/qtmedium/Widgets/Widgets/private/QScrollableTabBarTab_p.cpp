#include "QScrollableTabBarTab_p.h"
#include "QScrollableTabBarTab.h"

#include <QDebug>
#include <QFontMetrics>
#include <QPushButton>

QScrollableTabBarTabPrivate::QScrollableTabBarTabPrivate() {
}

QScrollableTabBarTabPrivate::~QScrollableTabBarTabPrivate() {
}

void QScrollableTabBarTabPrivate::init() {
    Q_Q(QScrollableTabBarTab);
    q->setAttribute(Qt::WA_StyledBackground);

    layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);

    iconSize = QSize(12, 12);
    iconMargins = QMargins(0, 0, 0, 0);
    textMargins = QMargins(0, 0, 0, 0);

    iconTextItem = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Preferred);

    closeButton = new QPushButton();
    closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    closeButton->setObjectName("close-button");

    layout->addItem(iconTextItem);
    layout->addWidget(closeButton);

    q->setLayout(layout);
    q->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void QScrollableTabBarTabPrivate::updateIconAndText() {
    Q_Q(QScrollableTabBarTab);

    QFontMetrics font(q->font());
    QSize fontSize(font.horizontalAdvance(text), font.height());

    int w =
        (icon.isNull() ? 0 : (iconSize.width() + iconMargins.left() + iconMargins.right())) +
        (text.isEmpty() ? 0 : (fontSize.width() + textMargins.left() + textMargins.right() * 2));
    int h =
        qMax(icon.isNull() ? 0 : (iconSize.height() + iconMargins.top() + iconMargins.bottom()),
             text.isEmpty() ? 0 : (fontSize.height() + textMargins.top() + textMargins.bottom()));

    QSizePolicy policy = iconTextItem->sizePolicy();
    iconTextItem->changeSize(w, h, policy.horizontalPolicy(), policy.verticalPolicy());
    layout->invalidate();

    q->update();
}
