#include "CDockCard.h"
#include "CDockTabBar.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QResizeEvent>
#include <QStyle>
#include <QTimer>

#include "CMenu.h"
#include "QMView.h"

CDockCard::CDockCard(QWidget *parent) : CLTabButton(parent) {
    init();
}

CDockCard::CDockCard(const QString &text, QWidget *parent) : CLTabButton(text, parent) {
    init();
}

CDockCard::CDockCard(const QIcon &icon, const QString &text, QWidget *parent) : CLTabButton(icon, text, parent) {
    init();
}

CDockCard::~CDockCard() {
    if (!m_widget->parentWidget()) {
        m_widget->deleteLater();
    }
    if (!m_container->parentWidget()) {
        m_container->deleteLater();
    }
}

void CDockCard::init() {
    setCheckable(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);

    m_closing = false;

    m_container = nullptr;
    m_widget = nullptr;
    m_viewMode = DockPinned;

    m_dragOffset = QSize(10, 10);
    m_readyDrag = false;

    m_sizePolicyV = sizePolicy();
    m_sizePolicyH =
        QSizePolicy(m_sizePolicyV.verticalPolicy(), m_sizePolicyV.horizontalPolicy(), m_sizePolicyV.controlType());
    setOrientation(Qt::Horizontal);
}

Qt::Orientation CDockCard::orientation() const {
    return m_orientation;
}

void CDockCard::setOrientation(Qt::Orientation orient) {
    m_orientation = orient;
    setSizePolicy((m_orientation == Qt::Horizontal) ? m_sizePolicyH : m_sizePolicyV);
    adjustSize();
}

QSize CDockCard::sizeHint() const {
    return (m_orientation == Qt::Horizontal) ? CTabButton::sizeHint() : CLTabButton::sizeHint();
}

QSize CDockCard::minimumSizeHint() const {
    return sizeHint();
}

int CDockCard::widthHint() const {
    return CTabButton::sizeHint().height();
}

QSize CDockCard::dragOffset() const {
    return m_dragOffset;
}

void CDockCard::setDragOffset(const QSize &dragOffset) {
    m_dragOffset = dragOffset;
    emit dragOffsetChanged();
}

QPixmap CDockCard::cardShot() const {
    QPixmap pixmap = QMView::createDeviceRenderPixmap(window()->windowHandle(), size());
    pixmap.fill(Qt::transparent);
    const_cast<CDockCard *>(this)->render(&pixmap);
    return pixmap;
}

CDockTabBar *CDockCard::tabBar() const {
    return qobject_cast<CDockTabBar *>(parentWidget());
}

QWidget *CDockCard::widget() const {
    return m_widget;
}

QWidget *CDockCard::container() const {
    return m_container;
}

QWidget *CDockCard::takeWidget() {
    if (!m_container) {
        return nullptr;
    }

    auto layout = m_container->layout();

    // Remove old widget
    auto w = m_widget;
    if (layout->count() > 0) {
        layout->removeWidget(w);
        w->setParent(nullptr);
    }
    w->removeEventFilter(this);

    // Delete container
    delete m_container;
    m_container = nullptr;
    m_widget = nullptr;

    return w;
}

void CDockCard::setWidget(QWidget *widget) {
    QLayout *layout;
    if (!m_container) {
        // Create container
        m_container = new QWidget();
        m_container->setObjectName("dock_card_container");
        m_container->setAttribute(Qt::WA_StyledBackground);

        layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);

        m_container->setLayout(layout);
    } else {
        layout = m_container->layout();
        if (layout->count() > 0) {
            layout->removeWidget(m_widget);
        }
        delete m_widget;
    }
    layout->addWidget(widget);
    widget->installEventFilter(this);

    m_widget = widget;
}

CDockCard::ViewMode CDockCard::viewMode() const {
    return m_viewMode;
}

static void adjustWindowGeometry(QWidget *w) {
    if (w->x() < 0 || w->y() < 0) {
        w->move(qMin(w->x(), 0), qMin(w->y(), 0));
    }

    if (w->x() + w->width() > QApplication::desktop()->width() ||
        w->y() + w->height() > QApplication::desktop()->height()) {
        w->move(qMin(w->x(), QApplication::desktop()->width() - w->width()),
                qMin(w->y(), QApplication::desktop()->height() - w->height()));
    }
}

void CDockCard::setViewMode(CDockCard::ViewMode viewMode) {
    if (m_viewMode == viewMode) {
        return;
    }

    auto layout = m_container->layout();
    switch (viewMode) {
        case DockPinned: {
            m_widget->setWindowFlags(Qt::Widget);
            layout->addWidget(m_widget);
            m_widget->setVisible(true);
            break;
        }
        case Float: {
            auto size = m_widget->size();
            layout->removeWidget(m_widget);
            m_widget->setParent(m_container);
            m_widget->setWindowFlags(Qt::Tool);
            if (m_viewMode == DockPinned) {
                if (!size.isEmpty())
                    m_widget->resize(size);
                moveWidget(mapToGlobal({0, 0}));
            }
            m_widget->setVisible(isChecked());
            break;
        }
        case Window: {
            auto size = m_widget->size();
            layout->removeWidget(m_widget);
            m_widget->setParent(nullptr);
            m_widget->setWindowFlags(Qt::Window);
            if (m_viewMode == DockPinned) {
                if (!size.isEmpty())
                    m_widget->resize(size);
                moveWidget(mapToGlobal({0, 0}));
            }
            m_widget->setVisible(isChecked());
            break;
        }
    }

    auto org = m_viewMode;
    m_viewMode = viewMode;

    emit viewModeChanged(viewMode, org);
}

void CDockCard::moveWidget(const QPoint &pos) {
    m_widget->move(pos);
    adjustWindowGeometry(m_widget);
}

void CDockCard::mousePressEvent(QMouseEvent *event) {
    CLTabButton::mousePressEvent(event);
    m_dragPos = event->pos();
    m_readyDrag = true;
}

void CDockCard::mouseMoveEvent(QMouseEvent *event) {
    CLTabButton::mouseMoveEvent(event);
    if (m_readyDrag) {
        QPoint pos = event->pos();
        if (qAbs(pos.x() - m_dragPos.x()) >= m_dragOffset.width() ||
            qAbs(pos.y() - m_dragPos.y()) >= m_dragOffset.height()) {
            m_readyDrag = false;
            emit startDrag(m_dragPos, cardShot());
        }
    }
}

void CDockCard::mouseReleaseEvent(QMouseEvent *event) {
    CLTabButton::mouseReleaseEvent(event);
    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CDockCard::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);

    auto dockPinnedAction = new QAction(tr("Dock pinned"), &menu);
    auto floatAction = new QAction(tr("Float"), &menu);
    auto windowAction = new QAction(tr("Window"), &menu);

    dockPinnedAction->setCheckable(true);
    floatAction->setCheckable(true);
    windowAction->setCheckable(true);

    switch (m_viewMode) {
        case DockPinned:
            dockPinnedAction->setChecked(true);
            break;
        case Float:
            floatAction->setChecked(true);
            break;
        case Window:
            windowAction->setChecked(true);
            break;
    }

    menu.addAction(dockPinnedAction);
    menu.addAction(floatAction);
    menu.addAction(windowAction);

    auto action = menu.exec(QCursor::pos());
    if (action == dockPinnedAction) {
        setViewMode(DockPinned);
    } else if (action == floatAction) {
        setViewMode(Float);
    } else if (action == windowAction) {
        setViewMode(Window);
    }
}

void CDockCard::leaveEvent(QEvent *event) {
    Q_UNUSED(event)
    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CDockCard::paintEvent(QPaintEvent *event) {
    return (m_orientation == Qt::Horizontal) ? CTabButton::paintEvent(event) : CLTabButton::paintEvent(event);
}

bool CDockCard::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_widget) {
        if (m_viewMode != DockPinned && event->type() == QEvent::Close) {
            m_closing = true;
            QTimer::singleShot(0, this, [this]() {
                m_closing = false; //
            });
        }
        if (event->type() == QEvent::Hide && m_closing) {
            // Close accepted
            setChecked(false);
        }
    }
    return QObject::eventFilter(obj, event);
}