#include "ANativeTitleBar_p.h"

#include <QWindow>

ANativeTitleBarPrivate::ANativeTitleBarPrivate() {
}

ANativeTitleBarPrivate::~ANativeTitleBarPrivate() {
}

void ANativeTitleBarPrivate::init() {
    w = nullptr;
    autoIcon = true;
    autoTitle = true;

    layout = new ANativeTitleBarLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    q_ptr->setLayout(layout);
}

QMenuBar *ANativeTitleBarPrivate::menuBar() const {
    return qobject_cast<QMenuBar *>(layout->menuWidget());
}

QLabel *ANativeTitleBarPrivate::titleLabel() const {
    return qobject_cast<QLabel *>(layout->labelWidget());
}

QAbstractButton *ANativeTitleBarPrivate::iconButton() const {
    return qobject_cast<QAbstractButton *>(layout->iconWidget());
}

QAbstractButton *ANativeTitleBarPrivate::minButton() const {
    return qobject_cast<QAbstractButton *>(layout->minButtonWidget());
}

QAbstractButton *ANativeTitleBarPrivate::maxButton() const {
    return qobject_cast<QAbstractButton *>(layout->maxButtonWidget());
}

QAbstractButton *ANativeTitleBarPrivate::closeButton() const {
    return qobject_cast<QAbstractButton *>(layout->closeButtonWidget());
}

void ANativeTitleBarPrivate::setMenuBar(QMenuBar *bar) {
    layout->setMenuWidget(bar);
}

void ANativeTitleBarPrivate::setTitleLabel(QLabel *label) {
    layout->setLabelWidget(label);
}

void ANativeTitleBarPrivate::setIconButton(QAbstractButton *btn) {
    layout->setIconWidget(btn);
}

void ANativeTitleBarPrivate::setMinButton(QAbstractButton *btn) {
    layout->setMinButtonWidget(btn);
}

void ANativeTitleBarPrivate::setMaxButton(QAbstractButton *btn) {
    layout->setMaxButtonWidget(btn);
}

void ANativeTitleBarPrivate::setCloseButton(QAbstractButton *btn) {
    layout->setCloseButtonWidget(btn);
}

QMenuBar *ANativeTitleBarPrivate::takeMenuBar() {
    return qobject_cast<QMenuBar *>(layout->takeMenuWidget());
}

QLabel *ANativeTitleBarPrivate::takeTitleLabel() {
    return qobject_cast<QLabel *>(layout->takeLabelWidget());
}

QAbstractButton *ANativeTitleBarPrivate::takeIconButton() {
    return qobject_cast<QAbstractButton *>(layout->takeIconWidget());
}

QAbstractButton *ANativeTitleBarPrivate::takeMinButton() {
    return qobject_cast<QAbstractButton *>(layout->takeMinButtonWidget());
}

QAbstractButton *ANativeTitleBarPrivate::takeMaxButton() {
    return qobject_cast<QAbstractButton *>(layout->takeMaxButtonWidget());
}

QAbstractButton *ANativeTitleBarPrivate::takeCloseButton() {
    return qobject_cast<QAbstractButton *>(layout->takeCloseButtonWidget());
}
