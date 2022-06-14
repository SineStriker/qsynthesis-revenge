#include "CWindowBarV2.h"

#include <QApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QRandomGenerator>

CWindowBarV2::CWindowBarV2(QMenuBar *menuBar, QWidget *parent) : CBaseTitleBarV2(parent) {
    m_titleMargin = 20;

    m_iconButton = new CToolButton();
    m_iconButton->setObjectName("icon-button");

    m_titleLabel = new QLabel();
    m_titleLabel->setObjectName("win-title-label");
    m_titleLabel->installEventFilter(this);

    m_minButton = new CToolButton();
    m_maxButton = new CToolButton();
    m_closeButton = new CToolButton();

    m_minButton->setObjectName("min-button");
    m_maxButton->setObjectName("max-button");
    m_closeButton->setObjectName("win-close-button");

    m_titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_menuBar = menuBar ? menuBar : new QMenuBar();
    m_menuBar->setObjectName("win-menu-bar");

    connect(m_maxButton, &QAbstractButton::toggled, this, &CWindowBarV2::reloadMaxButtonState);

    setIconButton(m_iconButton);
    setTitleLabel(m_titleLabel);
    setMinButton(m_minButton);
    setMaxButton(m_maxButton);
    setCloseButton(m_closeButton);
    setMenuBar(m_menuBar);

    reloadStrings();
}

CWindowBarV2::~CWindowBarV2() {
}

void CWindowBarV2::reloadStrings() {
    m_minButton->setToolTip(tr("Minimize"));
    m_closeButton->setToolTip(tr("Close"));
    reloadMaxButtonState();
}

void CWindowBarV2::reloadMaxButtonState(bool checked) {
    Q_UNUSED(checked)
    if (window()->windowState() == Qt::WindowMaximized) {
        m_maxButton->setChecked(true);
        m_maxButton->setToolTip(tr("Restore%1").arg(""));
    } else {
        m_maxButton->setChecked(false);
        m_maxButton->setToolTip(tr("Maximize"));
    }
}

QPixelSize CWindowBarV2::titleMargin() const {
    return m_titleMargin;
}

void CWindowBarV2::setTitleMargin(const QPixelSize &titleMargin) {
    m_titleMargin = titleMargin;
    update();
    emit styleChanged();
}

void CWindowBarV2::drawCentralTitle(QPainter &painter) {
    auto w = widget();
    if (!w) {
        return;
    }

    QString title = w->windowTitle();
    QFontMetrics font(m_titleLabel->font());
    int offset = m_titleMargin.value();

    int textWidth = font.horizontalAdvance(title);

    int limitWidth = m_titleLabel->width() - 2 * offset;
    int limitLeft = m_titleLabel->x() + offset;

    if (textWidth > limitWidth) {
        title = font.elidedText(title, Qt::ElideRight, limitWidth);
        textWidth = font.horizontalAdvance(title);
    }

    int expectLeft = (width() - textWidth) / 2;
    if (expectLeft < limitLeft || expectLeft + textWidth > limitLeft + limitWidth) {
        expectLeft = limitLeft + (limitWidth - textWidth) / 2;
    }

    QRect rect(expectLeft, m_titleLabel->y(), textWidth, m_titleLabel->height());
    painter.setPen(QPen(palette().windowText().color()));
    painter.setFont(m_titleLabel->font());
    painter.drawText(rect, Qt::AlignCenter, title);
}

void CWindowBarV2::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setClipRegion(event->region());
    painter.setRenderHint(QPainter::Antialiasing);
    drawCentralTitle(painter);
}

bool CWindowBarV2::eventFilter(QObject *obj, QEvent *event) {
    auto w = widget();
    if (obj == w) {
        switch (event->type()) {
        case QEvent::WindowTitleChange:
            update();
            break;
        default:
            break;
        }
    }
    return CBaseTitleBarV2::eventFilter(obj, event);
}
