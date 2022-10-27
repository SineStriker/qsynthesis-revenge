#include "ANativeTitleBar.h"

#include <QDebug>
#include <QEvent>
#include <QResizeEvent>
#include <QWindow>

#include "private/ANativeTitleBar_p.h"

ANativeTitleBar::ANativeTitleBar(QWidget *parent)
    : ANativeTitleBar(*new ANativeTitleBarPrivate(), parent) {
}

ANativeTitleBar::~ANativeTitleBar() {
}

QMenuBar *ANativeTitleBar::menuBar() const {
    return d_ptr->menuBar();
}

QLabel *ANativeTitleBar::titleLabel() const {
    return d_ptr->titleLabel();
}

QAbstractButton *ANativeTitleBar::iconButton() const {
    return d_ptr->iconButton();
}

QAbstractButton *ANativeTitleBar::minButton() const {
    return d_ptr->minButton();
}

QAbstractButton *ANativeTitleBar::maxButton() const {
    return d_ptr->maxButton();
}

QAbstractButton *ANativeTitleBar::closeButton() const {
    return d_ptr->closeButton();
}

void ANativeTitleBar::setMenuBar(QMenuBar *bar) {
    auto org = takeMenuBar();
    if (org)
        org->deleteLater();
    d_ptr->setMenuBar(bar);
    bar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    emit eleInstalled(bar);
}

void ANativeTitleBar::setTitleLabel(QLabel *label) {
    auto org = takeTitleLabel();
    if (org)
        org->deleteLater();
    d_ptr->setTitleLabel(label);
    if (d_ptr->autoTitle && d_ptr->w)
        label->setText(d_ptr->w->windowTitle());
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void ANativeTitleBar::setIconButton(QAbstractButton *btn) {
    auto org = takeIconButton();
    if (org)
        org->deleteLater();
    d_ptr->setIconButton(btn);
    if (d_ptr->autoIcon && d_ptr->w)
        btn->setIcon(d_ptr->w->windowIcon());
    btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    emit eleInstalled(btn);
}

void ANativeTitleBar::setMinButton(QAbstractButton *btn) {
    auto org = takeMinButton();
    if (org)
        org->deleteLater();
    d_ptr->setMinButton(btn);
    connect(btn, &QAbstractButton::clicked, this, &ANativeTitleBar::q_minButtonClicked);
    emit eleInstalled(btn);
}

void ANativeTitleBar::setMaxButton(QAbstractButton *btn) {
    auto org = takeMaxButton();
    if (org)
        org->deleteLater();
    d_ptr->setMaxButton(btn);
    btn->setCheckable(true);
    connect(btn, &QAbstractButton::clicked, this, &ANativeTitleBar::q_maxButtonClicked);
    if (d_ptr->w)
        btn->setChecked(d_ptr->w->isMaximized());
    emit eleInstalled(btn);
}

void ANativeTitleBar::setCloseButton(QAbstractButton *btn) {
    auto org = takeCloseButton();
    if (org)
        org->deleteLater();
    d_ptr->setCloseButton(btn);
    connect(btn, &QAbstractButton::clicked, this, &ANativeTitleBar::q_closeButtonClicked);
    emit eleInstalled(btn);
}

QMenuBar *ANativeTitleBar::takeMenuBar() {
    auto bar = d_ptr->takeMenuBar();
    if (bar) {
        emit eleRemoved(bar);
    }
    return bar;
}

QLabel *ANativeTitleBar::takeTitleLabel() {
    return d_ptr->takeTitleLabel();
}

QAbstractButton *ANativeTitleBar::takeIconButton() {
    auto btn = d_ptr->takeIconButton();
    if (btn) {
        emit eleRemoved(btn);
    }
    return btn;
}

QAbstractButton *ANativeTitleBar::takeMinButton() {
    auto btn = d_ptr->takeMinButton();
    if (btn) {
        disconnect(btn, &QAbstractButton::clicked, this, &ANativeTitleBar::q_minButtonClicked);
        emit eleRemoved(btn);
    }
    return btn;
}

QAbstractButton *ANativeTitleBar::takeMaxButton() {
    auto btn = d_ptr->takeMaxButton();
    if (btn) {
        disconnect(btn, &QAbstractButton::clicked, this, &ANativeTitleBar::q_maxButtonClicked);
        emit eleRemoved(btn);
    }
    return btn;
}

QAbstractButton *ANativeTitleBar::takeCloseButton() {
    auto btn = d_ptr->takeCloseButton();
    if (btn) {
        disconnect(btn, &QAbstractButton::clicked, this, &ANativeTitleBar::q_closeButtonClicked);
        emit eleRemoved(btn);
    }
    return btn;
}

QWidget *ANativeTitleBar::widget() const {
    return d_ptr->w;
}

void ANativeTitleBar::setWidget(QWidget *w) {
    QWidget *org = widget();
    if (org) {
        org->removeEventFilter(this);
    }
    d_ptr->w = w;
    if (w) {
        w->installEventFilter(this);
    }
}

bool ANativeTitleBar::titleFollowWindow() const {
    return d_ptr->autoTitle;
}

void ANativeTitleBar::setTitleFollowWindow(bool value) {
    d_ptr->autoTitle = value;
}

bool ANativeTitleBar::iconFollowWindow() const {
    return d_ptr->autoIcon;
}

void ANativeTitleBar::setIconFollowWindow(bool value) {
    d_ptr->autoIcon = value;
}

ANativeTitleBar::ANativeTitleBar(ANativeTitleBarPrivate &d, QWidget *parent)
    : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
    setAttribute(Qt::WA_StyledBackground);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void ANativeTitleBar::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}

bool ANativeTitleBar::eventFilter(QObject *obj, QEvent *event) {
    QWidget *w = widget();
    if (obj == w) {
        QAbstractButton *iconBtn = iconButton();
        QLabel *label = titleLabel();
        QAbstractButton *maxBtn = maxButton();
        switch (event->type()) {
            case QEvent::WindowIconChange:
                if (d_ptr->autoIcon && iconBtn)
                    iconBtn->setIcon(w->windowIcon());
                break;
            case QEvent::WindowTitleChange:
                if (d_ptr->autoTitle && label)
                    label->setText(w->windowTitle());
                break;
            case QEvent::WindowStateChange:
                if (maxBtn) {
                    maxBtn->setChecked(w->isMaximized());
                }
                break;
            default:
                break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ANativeTitleBar::q_minButtonClicked() {
    QWidget *w = widget();
    if (w) {
        w->showMinimized();
    }
}

void ANativeTitleBar::q_maxButtonClicked() {
    QWidget *w = widget();
    if (w) {
        if (w->isMaximized() || w->isFullScreen()) {
            w->showNormal();
        } else {
            w->showMaximized();
        }
    }
}

void ANativeTitleBar::q_closeButtonClicked() {
    QWidget *w = widget();
    if (w) {
        w->close();
    }
}
