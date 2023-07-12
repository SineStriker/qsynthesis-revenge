#include "CToolButton.h"

#include <QDateTime>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>

#include "QMCss.h"
#include "QMView.h"

class CToolButtonPrivate {
    Q_DECLARE_PUBLIC(CToolButton)
public:
    CToolButton *q_ptr;

    QSvgUri m_svgUris[7];

    QIcon m_iconUp;
    QIcon m_iconOver;
    QIcon m_iconDown;
    QIcon m_iconUpChecked;
    QIcon m_iconOverChecked;
    QIcon m_iconDownChecked;
    QIcon m_iconDisabled;

    bool m_autoCheck;

    void init() {
#ifdef QS_NO_TAB_FOCUS
        QS_REMOVE_TAB_FOCUS(q_ptr)
#endif
        m_autoCheck = true;
    }

    void reloadIcon() {
        Q_Q(CToolButton);
        if (!q->isEnabled() && !m_iconDisabled.isNull()) {
            q->setIcon(m_iconDisabled);
            return;
        }
        if (q->isChecked()) {
            if (q->isDown() && !m_iconDownChecked.isNull()) {
                q->setIcon(m_iconDownChecked);
                return;
            } else if (q->underMouse() && !m_iconOverChecked.isNull()) {
                q->setIcon(m_iconOverChecked);
                return;
            } else if (!m_iconUpChecked.isNull()) {
                q->setIcon(m_iconUpChecked);
                return;
            }
        }
        if (q->isDown() && !m_iconDown.isNull()) {
            q->setIcon(m_iconDown);
            return;
        } else if (q->underMouse() && !m_iconOver.isNull()) {
            q->setIcon(m_iconOver);
            return;
        } else if (!m_iconUp.isNull()) {
            q->setIcon(m_iconUp);
            return;
        } else {
            q->setIcon(QIcon());
        }
    }
};

CToolButton::CToolButton(QWidget *parent) : CToolButton(*new CToolButtonPrivate(), parent) {
}

CToolButton::CToolButton(const QString &text, QWidget *parent) : CToolButton(parent) {
    setText(text);
}

CToolButton::CToolButton(const QIcon &icon, const QString &text, QWidget *parent) : CToolButton(parent) {
    setIcon(icon);
    setText(text);
}

CToolButton::~CToolButton() {
}

QSvgUri CToolButton::iconUp() const {
    Q_D(const CToolButton);
    return d->m_svgUris[0];
}

void CToolButton::setIconUp(const QSvgUri &iconUp) {
    Q_D(CToolButton);

    d->m_svgUris[0] = iconUp;
    d->m_iconUp = iconUp.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconOver() const {
    Q_D(const CToolButton);
    return d->m_svgUris[1];
}

void CToolButton::setIconOver(const QSvgUri &iconOver) {
    Q_D(CToolButton);

    d->m_svgUris[1] = iconOver;
    d->m_iconOver = iconOver.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconDown() const {
    Q_D(const CToolButton);
    return d->m_svgUris[2];
}

void CToolButton::setIconDown(const QSvgUri &iconDown) {
    Q_D(CToolButton);

    d->m_svgUris[2] = iconDown;
    d->m_iconDown = iconDown.toIcon();
    d->reloadIcon();
    emit iconChanged();
}
QSvgUri CToolButton::iconUpChecked() const {
    Q_D(const CToolButton);
    return d->m_svgUris[3];
}

void CToolButton::setIconUpChecked(const QSvgUri &iconUpChecked) {
    Q_D(CToolButton);

    d->m_svgUris[3] = iconUpChecked;
    d->m_iconUpChecked = iconUpChecked.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconOverChecked() const {
    Q_D(const CToolButton);
    return d->m_svgUris[4];
}

void CToolButton::setIconOverChecked(const QSvgUri &iconOverChecked) {
    Q_D(CToolButton);

    d->m_svgUris[4] = iconOverChecked;
    d->m_iconOverChecked = iconOverChecked.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconDownChecked() const {
    Q_D(const CToolButton);
    return d->m_svgUris[5];
}

void CToolButton::setIconDownChecked(const QSvgUri &iconDownChecked) {
    Q_D(CToolButton);

    d->m_svgUris[5] = iconDownChecked;
    d->m_iconDownChecked = iconDownChecked.toIcon();
    d->reloadIcon();
    emit iconChanged();
}

QSvgUri CToolButton::iconDisabled() const {
    Q_D(const CToolButton);
    return d->m_svgUris[6];
}

void CToolButton::setIconDisabled(const QSvgUri &iconDisabled) {
    Q_D(CToolButton);

    d->m_svgUris[6] = iconDisabled;
    d->m_iconDisabled = iconDisabled.toIcon(QSvgUri::Normal | QSvgUri::Disabled);
    d->reloadIcon();
    emit iconChanged();
}

bool CToolButton::autoCheck() const {
    Q_D(const CToolButton);
    return d->m_autoCheck;
}

void CToolButton::setAutoCheck(bool autoCheck) {
    Q_D(CToolButton);

    d->m_autoCheck = autoCheck;
}

CToolButton::CToolButton(CToolButtonPrivate &d, QWidget *parent) : QToolButton(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}

bool CToolButton::event(QEvent *event) {
    Q_D(CToolButton);
    switch (event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::Enter:
        case QEvent::Leave:
        case QEvent::EnabledChange:
            d->reloadIcon();
            break;
        default:
            break;
    }
    return QToolButton::event(event);
}

void CToolButton::paintEvent(QPaintEvent *event) {
    QStylePainter p(this);
    QStyleOptionToolButton option;
    initStyleOption(&option);

    // Correct icon color
    QSvgUri::tryFallbackIconColor(option.icon, [this]() { return QMCss::ColorToCssString(currentTextColor()); });
    p.drawComplexControl(QStyle::CC_ToolButton, option);
}

void CToolButton::nextCheckState() {
    Q_D(CToolButton);
    if (d->m_autoCheck) {
        QToolButton::nextCheckState();
        d->reloadIcon();
    }
}

void CToolButton::checkStateSet() {
    Q_D(CToolButton);

    QToolButton::checkStateSet();
    d->reloadIcon();
}

QColor CToolButton::currentTextColor(const QSize &hint) const {
    auto size = hint.isEmpty() ? minimumSizeHint() : hint;

    QStyleOptionToolButton option2;
    initStyleOption(&option2);
    option2.rect.setSize(size);
    option2.text = QChar(0x25A0);
    option2.icon = {};
    option2.iconSize = {};

    QPixmap pixmap = QMView::createDeviceRenderPixmap(window()->windowHandle(), size);
    pixmap.fill(Qt::transparent);
    QPainter p2(&pixmap);

    // If padding is set, the following statement won't work in hover/pressed state
    // As a result, translucent text color will cause accuracy error
    // style()->drawControl(QStyle::CE_ToolButtonLabel, &option2, &p2, static_cast<QToolButton *>(this));
    style()->drawComplexControl(QStyle::CC_ToolButton, &option2, &p2, this);

    QImage image = pixmap.toImage();
    int centerX = image.width() / 2;
    int centerY = image.height() / 2;
    QColor color = image.pixelColor(centerX, centerY);

    return color;
}