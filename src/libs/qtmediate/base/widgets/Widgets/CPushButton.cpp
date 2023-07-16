#include "CPushButton.h"

#include <QDateTime>
#include <QDebug>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>

#include "QMCss.h"
#include "QMView.h"

#include "private/IconColorImpl.h"

CPushButton::CPushButton(QWidget *parent) : QPushButton(parent) {
}

CPushButton::CPushButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
}

CPushButton::CPushButton(const QIcon &icon, const QString &text, QWidget *parent) : QPushButton(icon, text, parent) {
}

CPushButton::~CPushButton() {
}

void CPushButton::paintEvent(QPaintEvent *event) {
    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);

    // Correct icon color
    IconColorImpl::correctIconStateAndColor(option.icon, IconColorImpl::getButtonClickState(this),
                                            IconColorImpl::defaultSalt(this), [this]() {
                                                return QMCss::ColorToCssString(currentTextColor()); //
                                            });

    p.drawControl(QStyle::CE_PushButton, option);
}

QColor CPushButton::currentTextColor(const QSize &hint) const {
    auto size = hint.isEmpty() ? minimumSizeHint() : hint;

    QStyleOptionButton option2;
    initStyleOption(&option2);
    option2.rect.setSize(size);
    option2.text = QChar(0x25A0);
    option2.icon = {};
    option2.iconSize = {};

    QPen pen;
    IconColorImpl::getTextColor(pen, size, [&](QPainter *painter) {
        style()->drawControl(QStyle::CE_PushButton, &option2, painter, this); //
    });

    return pen.color();
}