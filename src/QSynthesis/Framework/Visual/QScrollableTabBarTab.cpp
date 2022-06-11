#include "QScrollableTabBarTab.h"
#include "QScrollableTabBarTab_p.h"

#include <QDebug>
#include <QMouseEvent>
#include <QStyle>

QScrollableTabBarTab::QScrollableTabBarTab(QWidget *parent)
    : QScrollableTabBarTab(*new QScrollableTabBarTabPrivate(), parent) {
}

QScrollableTabBarTab::~QScrollableTabBarTab() {
}

QIcon QScrollableTabBarTab::icon() const {
    Q_D(const QScrollableTabBarTab);
    return d->iconButton->icon();
}

void QScrollableTabBarTab::setIcon(const QIcon &icon) {
    Q_D(QScrollableTabBarTab);
    d->iconButton->setIcon(icon);
    if (icon.isNull()) {
        d->iconButton->hide();
    } else {
        d->iconButton->show();
    }
    emit styleChanged();
}

QSize QScrollableTabBarTab::iconSize() const {
    Q_D(const QScrollableTabBarTab);
    return d->iconButton->iconSize();
}

void QScrollableTabBarTab::setIconSize(const QSize &iconSize) {
    Q_D(QScrollableTabBarTab);
    d->iconButton->setIconSize(iconSize);
    emit styleChanged();
}

QString QScrollableTabBarTab::text() const {
    Q_D(const QScrollableTabBarTab);
    return d->textLabel->text();
}

void QScrollableTabBarTab::setText(const QString &text) {
    Q_D(QScrollableTabBarTab);
    d->textLabel->setText(text);
    if (text.isEmpty()) {
        d->textLabel->hide();
    } else {
        d->textLabel->show();
    }
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
    setProperty("selected", selected);
    style()->polish(this);
}

QAbstractButton *QScrollableTabBarTab::closeButton() const {
    Q_D(const QScrollableTabBarTab);
    return d->closeButton;
}

QScrollableTabBarTab::QScrollableTabBarTab(QScrollableTabBarTabPrivate &d, QWidget *parent)
    : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();

    setProperty("selected", false);
}
