#include "CDockCard.h"
#include "private/CDockCard_p.h"
#include "private/CDockTabBar.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QResizeEvent>
#include <QStyle>
#include <QTimer>

#include "QMView.h"

CDockCard::CDockCard(QWidget *parent) : CLTabButton(parent), d(new CDockCardPrivate(this)) {
    d->init();
}

CDockCard::CDockCard(const QString &text, QWidget *parent) : CLTabButton(text, parent), d(new CDockCardPrivate(this)) {
    d->init();
}

CDockCard::CDockCard(const QIcon &icon, const QString &text, QWidget *parent)
    : CLTabButton(icon, text, parent), d(new CDockCardPrivate(this)) {
    d->init();
}

CDockCard::~CDockCard() {
    delete d;
}

Qt::Orientation CDockCard::orientation() const {
    return d->m_orientation;
}

void CDockCard::setOrientation(Qt::Orientation orient) {
    d->m_orientation = orient;
    setSizePolicy((d->m_orientation == Qt::Horizontal) ? d->m_sizePolicyH : d->m_sizePolicyV);
    adjustSize();
}

QSize CDockCard::sizeHint() const {
    return (d->m_orientation == Qt::Horizontal) ? CTabButton::sizeHint() : CLTabButton::sizeHint();
}

QSize CDockCard::minimumSizeHint() const {
    return sizeHint();
}

int CDockCard::widthHint() const {
    return CTabButton::sizeHint().height();
}

QSize CDockCard::dragOffset() const {
    return d->m_dragOffset;
}

void CDockCard::setDragOffset(const QSize &dragOffset) {
    d->m_dragOffset = dragOffset;
}

QWidget *CDockCard::widget() const {
    return d->m_widget;
}

QWidget *CDockCard::container() const {
    return d->m_container;
}

QWidget *CDockCard::takeWidget() {
    auto &m_widget = d->m_widget;
    auto &m_container = d->m_container;

    if (!d->m_container) {
        return nullptr;
    }

    auto layout = d->m_container->layout();

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
    auto &m_widget = d->m_widget;
    auto &m_container = d->m_container;

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
    return d->m_viewMode;
}

static void adjustWindowGeometry(QWidget *w) {
    if (w->x() < 0 || w->y() < 0) {
        w->move(qMax(w->x(), 0), qMax(w->y(), 0));
    }

    if (w->x() + w->width() > QApplication::desktop()->width() ||
        w->y() + w->height() > QApplication::desktop()->height()) {
        w->move(qMin(w->x(), QApplication::desktop()->width() - w->width()),
                qMin(w->y(), QApplication::desktop()->height() - w->height()));
    }
}

void CDockCard::setViewMode(CDockCard::ViewMode viewMode) {
    auto &m_viewMode = d->m_viewMode;
    auto &m_widget = d->m_widget;
    auto &m_container = d->m_container;

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

CDockTabBar *CDockCard::tabBar() const {
    return d->m_tabBar;
}

void CDockCard::moveWidget(const QPoint &pos) {
    auto &m_widget = d->m_widget;

    m_widget->move(pos);
    adjustWindowGeometry(m_widget);
}

void CDockCard::mousePressEvent(QMouseEvent *event) {
    auto &m_dragPos = d->m_dragPos;
    auto &m_readyDrag = d->m_readyDrag;

    CLTabButton::mousePressEvent(event);
    m_dragPos = event->pos();
    m_readyDrag = true;
}

void CDockCard::mouseMoveEvent(QMouseEvent *event) {
    auto &m_dragPos = d->m_dragPos;
    auto &m_dragOffset = d->m_dragOffset;
    auto &m_readyDrag = d->m_readyDrag;

    CLTabButton::mouseMoveEvent(event);
    if (m_readyDrag) {
        QPoint pos = event->pos();
        if (qAbs(pos.x() - m_dragPos.x()) >= m_dragOffset.width() ||
            qAbs(pos.y() - m_dragPos.y()) >= m_dragOffset.height()) {
            m_readyDrag = false;

            if (auto tabBar = this->tabBar()) {
                emit tabBar->dragStarted(this, m_dragPos, d->cardShot());
            }
        }
    }
}

void CDockCard::mouseReleaseEvent(QMouseEvent *event) {
    auto &m_readyDrag = d->m_readyDrag;

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

    switch (d->m_viewMode) {
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

    auto &m_readyDrag = d->m_readyDrag;

    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CDockCard::paintEvent(QPaintEvent *event) {
    return (d->m_orientation == Qt::Horizontal) ? CTabButton::paintEvent(event) : CLTabButton::paintEvent(event);
}

bool CDockCard::eventFilter(QObject *obj, QEvent *event) {
    if (obj == d->m_widget) {
        if (event->type() == QEvent::Close) {
            d->m_closing = true;
            QTimer::singleShot(0, this, [this]() {
                d->m_closing = false; //
            });
        }
        if (event->type() == QEvent::Hide && d->m_closing) {
            // Close accepted
            setChecked(false);
            if (d->m_viewMode == DockPinned) {
                QTimer::singleShot(0, this, [this]() {
                    d->m_widget->show(); //
                });
            }
        }
    }
    return QObject::eventFilter(obj, event);
}