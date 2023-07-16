#include "CToolButton.h"

#include <QDateTime>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>

#include "QMCss.h"
#include "QMView.h"

#include "private/IconColorImpl.h"

CToolButton::CToolButton(QWidget *parent) : QToolButton(parent) {
}

CToolButton::CToolButton(const QString &text, QWidget *parent) : QToolButton(parent) {
    setText(text);
}

CToolButton::CToolButton(const QIcon &icon, const QString &text, QWidget *parent) : QToolButton(parent) {
    setIcon(icon);
    setText(text);
}

CToolButton::~CToolButton() {
}

void CToolButton::paintEvent(QPaintEvent *event) {
    QStylePainter p(this);
    QStyleOptionToolButton option;
    initStyleOption(&option);

    // Correct icon color
    IconColorImpl::correctIconStateAndColor(option.icon, IconColorImpl::getButtonClickState(this),
                                            IconColorImpl::defaultSalt(this), [this]() {
                                                return QMCss::ColorToCssString(currentTextColor()); //
                                            });

    p.drawComplexControl(QStyle::CC_ToolButton, option);
}

QColor CToolButton::currentTextColor(const QSize &hint) const {
    auto size = hint.isEmpty() ? minimumSizeHint() : hint;

    QStyleOptionToolButton option2;
    initStyleOption(&option2);
    option2.rect.setSize(size);
    option2.text = QChar(0x25A0);
    option2.icon = {};
    option2.iconSize = {};

    QPen pen;
    IconColorImpl::getTextColor(pen, size, [&](QPainter *painter) {
        style()->drawComplexControl(QStyle::CC_ToolButton, &option2, painter, this); //
    });

    return pen.color();
}