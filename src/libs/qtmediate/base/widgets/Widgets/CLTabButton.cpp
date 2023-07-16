#include "CLTabButton.h"
#include "CTabButton.h"

#include <QPainter>
#include <QResizeEvent>
#include <QStyleOptionButton>
#include <QStylePainter>

#include "QMCss.h"
#include "QMView.h"

#include "private/IconColorImpl.h"

CLTabButton::CLTabButton(QWidget *parent) : CTabButton(parent) {
    init();
}

CLTabButton::CLTabButton(const QString &text, QWidget *parent) : CTabButton(text, parent) {
    init();
}

CLTabButton::CLTabButton(const QIcon &icon, const QString &text, QWidget *parent) : CTabButton(icon, text, parent) {
    init();
}

CLTabButton::~CLTabButton() {
}

void CLTabButton::init() {
    m_longitudinalDirection = TopToBottom;
    QSizePolicy sp = sizePolicy();
    setSizePolicy(QSizePolicy(sp.verticalPolicy(), sp.horizontalPolicy(), sp.controlType()));
}

QSize CLTabButton::sizeHint() const {
    QSize sz = QPushButton::sizeHint();
    QSize iconSz = iconSize();
    if (icon().isNull()) {
        iconSz = QSize(0, 0);
    }
    int offset = iconSz.height() * m_spaceRatio;
    return {sz.height(), sz.width() + offset};
}

QSize CLTabButton::minimumSizeHint() const {
    return sizeHint();
}

CLTabButton::LongitudinalDirection CLTabButton::longitudinalDirection() const {
    return m_longitudinalDirection;
}

void CLTabButton::setLongitudinalDirection(LongitudinalDirection d) {
    m_longitudinalDirection = d;
    adjustSize();
}

void CLTabButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    QRect rect = this->rect();
    QPixmap tvPix = QMView::createDeviceRenderPixmap(window()->windowHandle(), rect.size().transposed());
    tvPix.fill(Qt::transparent);

    QStylePainter pp(&tvPix, this);
    QStyleOptionButton option;
    initStyleOption(&option);

    // Correct icon color
    IconColorImpl::correctIconStateAndColor(option.icon, IconColorImpl::getButtonClickState(this),
                                            IconColorImpl::defaultSalt(this), [this]() {
                                                return QMCss::ColorToCssString(currentTextColor()); //
                                            });

    QSize sz = iconSize();
    QTransform tf;
    tf.rotate((m_longitudinalDirection == TopToBottom) ? -90 : 90);

    // Get the pixmap to apply with right size
    QPixmap tmp = option.icon.pixmap(sz).transformed(tf, Qt::SmoothTransformation);

    sz.rheight() *= 1 + m_spaceRatio; // Multiply width

    sz = QSize(sz.height(), sz.width());

    QPixmap exp(sz); // Expended
    exp.fill(Qt::transparent);

    QPainter painter(&exp);
    painter.drawPixmap(QRect(QPoint(), tmp.size()), tmp);

    option.icon = QIcon(exp);                          // Change to real icon
    option.iconSize = tmp.isNull() ? QSize(0, 0) : sz; // Change to real size
    option.rect = QRect(0, 0, rect.height(), rect.width());

    pp.drawControl(QStyle::CE_PushButton, option);

    QTransform tf2;
    tf2.rotate((m_longitudinalDirection == TopToBottom) ? 90 : -90);

    QPainter p(this);
    p.drawPixmap(rect, tvPix.transformed(tf2, Qt::SmoothTransformation));
}
