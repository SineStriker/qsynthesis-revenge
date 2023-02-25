#include "QScrollableTabBarTab.h"
#include "QScrollableTabBarTab_p.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QStyle>

QScrollableTabBarTab::QScrollableTabBarTab(QWidget *parent)
    : QScrollableTabBarTab(*new QScrollableTabBarTabPrivate(), parent) {
}

QScrollableTabBarTab::~QScrollableTabBarTab() {
}

QIcon QScrollableTabBarTab::icon() const {
    Q_D(const QScrollableTabBarTab);
    return d->icon;
}

void QScrollableTabBarTab::setIcon(const QIcon &icon) {
    Q_D(QScrollableTabBarTab);

    d->icon = icon;
    d->updateIconAndText();

    emit styleChanged();
}

QSize QScrollableTabBarTab::iconSize() const {
    Q_D(const QScrollableTabBarTab);
    return d->iconSize;
}

void QScrollableTabBarTab::setIconSize(const QSize &iconSize) {
    Q_D(QScrollableTabBarTab);

    d->iconSize = iconSize;
    d->updateIconAndText();

    emit styleChanged();
}

QMargins QScrollableTabBarTab::iconMargins() const {
    Q_D(const QScrollableTabBarTab);
    return d->iconMargins;
}

void QScrollableTabBarTab::setIconMargins(const QMargins &iconMargins) {
    Q_D(QScrollableTabBarTab);

    d->iconMargins = iconMargins;
    d->updateIconAndText();

    emit styleChanged();
}

QString QScrollableTabBarTab::text() const {
    Q_D(const QScrollableTabBarTab);
    return d->text;
}

void QScrollableTabBarTab::setText(const QString &text) {
    Q_D(QScrollableTabBarTab);

    d->text = text;
    d->updateIconAndText();

    emit styleChanged();
}

QMargins QScrollableTabBarTab::textMargins() const {
    Q_D(const QScrollableTabBarTab);
    return d->textMargins;
}

void QScrollableTabBarTab::setTextMargins(const QMargins &textMargins) {
    Q_D(QScrollableTabBarTab);

    d->textMargins = textMargins;
    d->updateIconAndText();

    emit styleChanged();
}

QVariant QScrollableTabBarTab::data() const {
    Q_D(const QScrollableTabBarTab);
    return d->data;
}

void QScrollableTabBarTab::setData(const QVariant &data) {
    Q_D(QScrollableTabBarTab);
    d->data = data;
}

bool QScrollableTabBarTab::selected() const {
    return property("selected").toBool();
}

void QScrollableTabBarTab::setSelected(bool selected) {
    Q_D(QScrollableTabBarTab);
    d->closeButton->setChecked(selected);

    setProperty("selected", selected);
    style()->polish(this);
    update();
}

QAbstractButton *QScrollableTabBarTab::closeButton() const {
    Q_D(const QScrollableTabBarTab);
    return d->closeButton;
}

void QScrollableTabBarTab::setCloseButton(QAbstractButton *button) {
    Q_D(QScrollableTabBarTab);
    d->layout->replaceWidget(d->closeButton, button);
    delete d->closeButton;
    d->closeButton = button;
}

QScrollableTabBarTab::QScrollableTabBarTab(QScrollableTabBarTabPrivate &d, QWidget *parent)
    : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();

    setProperty("selected", false);
}

void QScrollableTabBarTab::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipRegion(event->region());
    painter.translate(contentsRect().topLeft());

    Q_D(const QScrollableTabBarTab);

    int h = contentsRect().height();

    // Draw Icon
    if (!d->icon.isNull()) {
        painter.drawPixmap(
            QRect(QPoint(d->iconMargins.left(), (h - d->iconSize.height() + d->iconMargins.top() -
                                                 d->iconMargins.bottom()) /
                                                    2),
                  d->iconSize),
            d->icon.pixmap(d->iconSize));
    }

    // Draw Text
    if (!d->text.isEmpty()) {
        QFontMetrics font(this->font());
        QRect rect(d->iconMargins.left() + d->iconSize.width() + d->iconMargins.right() +
                       d->textMargins.left(),
                   (h - font.height() + d->textMargins.top() - d->textMargins.bottom()) / 2,
                   font.horizontalAdvance(d->text), font.height());
        if (d->icon.isNull()) {
            rect.moveLeft(d->textMargins.left());
        }
        painter.setPen(QPen(palette().windowText().color()));
        painter.setFont(this->font());
        painter.drawText(rect, Qt::AlignCenter, d->text);
    }

    QFrame::paintEvent(event);
}
