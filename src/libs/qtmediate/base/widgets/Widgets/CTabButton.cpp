#include "CTabButton.h"

#include <QDateTime>
#include <QDebug>
#include <QStyleOptionButton>
#include <QStylePainter>

#include "QMCss.h"
#include "QMView.h"

#include "private/IconColorImpl.h"

CTabButton::CTabButton(QWidget *parent) : CPushButton(parent) {
    init();
}

CTabButton::CTabButton(const QString &text, QWidget *parent) : CPushButton(text, parent) {
    init();
}

CTabButton::CTabButton(const QIcon &icon, const QString &text, QWidget *parent) : CPushButton(icon, text, parent) {
    init();
}

CTabButton::~CTabButton() {
}

void CTabButton::init() {
    m_spaceRatio = 0;
}

double CTabButton::spaceRatio() const {
    return m_spaceRatio;
}

void CTabButton::setSpaceRatio(double ratio) {
    m_spaceRatio = ratio;
    emit spaceChanged();
}

QSize CTabButton::sizeHint() const {
    QSize sz = QPushButton::sizeHint();
    QSize iconSz = iconSize();
    if (icon().isNull()) {
        iconSz = QSize(0, 0);
    }
    int offset = iconSz.width() * m_spaceRatio;
    return {sz.width() + offset, sz.height()};
}

QSize CTabButton::minimumSizeHint() const {
    return sizeHint();
}

void CTabButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);

    // Correct icon color
    IconColorImpl::correctIconStateAndColor(option.icon, IconColorImpl::getButtonClickState(this),
                                            IconColorImpl::defaultSalt(this), [this]() {
                                                return QMCss::ColorToCssString(currentTextColor()); //
                                            });

    QSize sz = iconSize();
    QPixmap tmp = option.icon.pixmap(sz); // Get the pixmap to apply with right size

    sz.rwidth() *= 1 + m_spaceRatio;      // Multiply width

    QPixmap exp = QMView::createDeviceRenderPixmap(window()->windowHandle(), sz); // Expended
    exp.fill(Qt::transparent);

    QPainter painter(&exp);
    painter.drawPixmap(QRect(QPoint(), iconSize()), tmp);

    option.icon = QIcon(exp);                          // Change to real icon
    option.iconSize = tmp.isNull() ? QSize(0, 0) : sz; // Change to real size

    p.drawControl(QStyle::CE_PushButton, option);      // From Qt source
}
