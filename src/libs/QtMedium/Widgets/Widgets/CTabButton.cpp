#include "CTabButton.h"

#include <QDateTime>
#include <QDebug>
#include <QStyleOptionButton>
#include <QStylePainter>

#include "QMView.h"

CTabButton::CTabButton(QWidget *parent) : CPushButton(parent) {
    init();
}

CTabButton::CTabButton(const QString &text, QWidget *parent) : CPushButton(text, parent) {
    init();
}

CTabButton::CTabButton(const QIcon &icon, const QString &text, QWidget *parent)
    : CPushButton(icon, text, parent) {
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
    if (paintedIcon().isNull()) {
        iconSz = QSize(0, 0);
    }
    int offset = iconSz.width() * m_spaceRatio;
    return QSize(sz.width() + offset, sz.height());
}

QSize CTabButton::minimumSizeHint() const {
    return sizeHint();
}

QIcon CTabButton::paintedIcon() const {
    return icon();
}

void CTabButton::paintEvent(QPaintEvent *event) {
    QSize sz = iconSize();
    QPixmap tmp = paintedIcon().pixmap(sz); // Get the pixmap to apply with right size

    sz.rwidth() *= 1 + m_spaceRatio; // Multiply width

    QPixmap exp = QMView::createDeviceRenderPixmap(window()->windowHandle(), sz); // Expended
    exp.fill(Qt::transparent);

    QPainter painter(&exp);
    painter.drawPixmap(QRect(QPoint(), iconSize()), tmp);

    QStylePainter p(this);
    QStyleOptionButton option;
    initStyleOption(&option);

    option.icon = QIcon(exp);                          // Change to real icon
    option.iconSize = tmp.isNull() ? QSize(0, 0) : sz; // Change to real size

    p.drawControl(QStyle::CE_PushButton, option); // From Qt source

    Q_UNUSED(event)
}
