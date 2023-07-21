#include "CDockCard.h"
#include "private/CDockCard_p.h"
#include "private/CDockFrame_p.h"
#include "private/CDockSideBar.h"
#include "private/CDockTabBar.h"
#include "private/CDockToolWindow_p.h"

#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QDesktopWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QStyle>
#include <QTimer>

#include <CMenu.h>

#include "QMView.h"

CDockCardPrivate::CDockCardPrivate() {
    m_closing = false;

    m_container = nullptr;
    m_widget = nullptr;
    m_viewMode = CDockCard::DockPinned;

    m_dragOffset = QSize(10, 10);
    m_readyDrag = false;

    m_tabBar = nullptr;
    m_floatingHelper = nullptr;
}

CDockCardPrivate::~CDockCardPrivate() {
    if (!m_widget->parentWidget()) {
        m_widget->deleteLater();
    }
    if (!m_container->parentWidget()) {
        m_container->deleteLater();
    }
}
void CDockCardPrivate::init() {
    Q_Q(CDockCard);
    m_sizePolicyV = q->sizePolicy();
    m_sizePolicyH =
        QSizePolicy(m_sizePolicyV.verticalPolicy(), m_sizePolicyV.horizontalPolicy(), m_sizePolicyV.controlType());

    q->setCheckable(true);
    q->setContextMenuPolicy(Qt::DefaultContextMenu);
    q->setOrientation(Qt::Horizontal); // orientation
}

QPixmap CDockCardPrivate::cardShot() const {
    Q_Q(const CDockCard);
    QPixmap pixmap = QMView::createDeviceRenderPixmap(q->window()->windowHandle(), q->size());
    pixmap.fill(Qt::transparent);
    const_cast<CDockCard *>(q)->render(&pixmap);
    return pixmap;
}

QMenu *CDockCardPrivate::createViewModeMenu(CDockCard *card) {
    auto menu = new CMenu(card);
    menu->setTitle(CDockCard::tr("View Mode"));

    auto dockPinnedAction = new QAction(CDockCard::tr("Dock pinned"), menu);
    auto floatAction = new QAction(CDockCard::tr("Float"), menu);
    auto windowAction = new QAction(CDockCard::tr("Window"), menu);

    dockPinnedAction->setCheckable(true);
    floatAction->setCheckable(true);
    windowAction->setCheckable(true);

    switch (card->viewMode()) {
        case CDockCard::DockPinned:
            dockPinnedAction->setChecked(true);
            break;
        case CDockCard::Float:
            floatAction->setChecked(true);
            break;
        case CDockCard::Window:
            windowAction->setChecked(true);
            break;
    }

    menu->addAction(dockPinnedAction);
    menu->addAction(floatAction);
    menu->addAction(windowAction);

    QObject::connect(dockPinnedAction, &QAction::triggered, menu, [card]() {
        card->setViewMode(CDockCard::DockPinned); //
    });
    QObject::connect(floatAction, &QAction::triggered, menu, [card]() {
        card->setViewMode(CDockCard::Float); //
    });
    QObject::connect(windowAction, &QAction::triggered, menu, [card]() {
        card->setViewMode(CDockCard::Window); //
    });

    return menu;
}

CDockCard::CDockCard(QWidget *parent) : CDockCard(*new CDockCardPrivate(), parent) {
}

CDockCard::CDockCard(const QString &text, QWidget *parent) : CDockCard(*new CDockCardPrivate(), parent) {
    setText(text);
}

CDockCard::CDockCard(const QIcon &icon, const QString &text, QWidget *parent) : CDockCard(text, parent) {
    setIcon(icon);
}

CDockCard::~CDockCard() {
}

Qt::Orientation CDockCard::orientation() const {
    Q_D(const CDockCard);
    return d->m_orientation;
}

void CDockCard::setOrientation(Qt::Orientation orient) {
    Q_D(CDockCard);
    d->m_orientation = orient;
    setSizePolicy((d->m_orientation == Qt::Horizontal) ? d->m_sizePolicyH : d->m_sizePolicyV);
    adjustSize();
}

QSize CDockCard::sizeHint() const {
    Q_D(const CDockCard);
    return (d->m_orientation == Qt::Horizontal) ? CTabButton::sizeHint() : CLTabButton::sizeHint();
}

QSize CDockCard::minimumSizeHint() const {
    return sizeHint();
}

int CDockCard::widthHint() const {
    return CTabButton::sizeHint().height();
}

QSize CDockCard::dragOffset() const {
    Q_D(const CDockCard);
    return d->m_dragOffset;
}

void CDockCard::setDragOffset(const QSize &dragOffset) {
    Q_D(CDockCard);
    d->m_dragOffset = dragOffset;
}

QWidget *CDockCard::widget() const {
    Q_D(const CDockCard);
    return d->m_widget;
}

QWidget *CDockCard::container() const {
    Q_D(const CDockCard);
    return d->m_container;
}

QWidget *CDockCard::takeWidget() {
    Q_D(CDockCard);
    auto &m_widget = d->m_widget;
    auto &m_container = d->m_container;

    if (!d->m_container) {
        return nullptr;
    }

    auto layout = d->m_container->layout();

    // Remove old widget
    auto &w = m_widget;
    if (layout->count() > 0) {
        layout->removeWidget(w);
        w->setParent(nullptr);
    }
    w->removeEventFilter(this);

    // Remove card reference
    {
        auto tw = qobject_cast<CDockToolWindow *>(w);
        if (tw) {
            tw->d_func()->setCard(nullptr);
        }
    }

    // Delete helper
    if (d->m_floatingHelper) {
        delete d->m_floatingHelper;
        d->m_floatingHelper = nullptr;
    }

    // Delete container
    delete m_container;
    m_container = nullptr;
    m_widget = nullptr;

    return w;
}

void CDockCard::setWidget(QWidget *widget) {
    Q_D(CDockCard);
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

        if (d->m_floatingHelper) {
            delete d->m_floatingHelper;
        }
    }
    layout->addWidget(widget);
    widget->installEventFilter(this);
    d->m_floatingHelper = new QMFloatingWindowHelper(widget, this);
    m_widget = widget;

    // Add card reference
    {
        auto tw = qobject_cast<CDockToolWindow *>(widget);
        if (tw) {
            tw->d_func()->setCard(this);
            tw->viewModeChanged(d->m_viewMode);
        }
    }
}

CDockCard::ViewMode CDockCard::viewMode() const {
    Q_D(const CDockCard);
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
    Q_D(CDockCard);
    auto &m_viewMode = d->m_viewMode;
    auto &m_widget = d->m_widget;
    auto &m_container = d->m_container;

    if (m_viewMode == viewMode) {
        return;
    }

    auto asWindow = [&](const QSize &size, const QPoint &extraOffset) {
        // Size
        if (!size.isEmpty()) {
            m_widget->resize(size);
        } else if (!d->oldGeometry.isEmpty()) {
            m_widget->resize(d->oldGeometry.size());
        }

        // Pos
        if (!d->oldGeometry.isEmpty()) {
            m_widget->move(d->oldGeometry.topLeft() - extraOffset);
        } else {
            QPoint offset;
            switch (d->m_tabBar->sideBar()->edge()) {
                case Qt::TopEdge:
                    offset.ry() += height();
                    break;
                case Qt::BottomEdge:
                    offset.ry() -= height() + size.height();
                    break;
                case Qt::LeftEdge:
                    offset.rx() += width();
                    break;
                case Qt::RightEdge:
                    offset.rx() -= width() + size.width();
                    break;
            }

            moveWidget(mapToGlobal({0, 0}) + offset - extraOffset);
        }

        m_widget->setVisible(isChecked());
    };

    auto layout = m_container->layout();
    switch (viewMode) {
        case DockPinned: {
            d->m_floatingHelper->setFloating(false);
            m_widget->setWindowFlags(Qt::Widget);
            layout->addWidget(m_widget);
            m_widget->setVisible(true);
            QTimer::singleShot(0, this, [d, this]() {
                setSize(d->oldGeometry.size()); //
            });
            break;
        }
        case Float: {
            auto size = m_widget->size();
            auto extraOffset =
                m_viewMode == DockPinned ? QPoint{0, 0} : (m_widget->mapToGlobal({0, 0}) - m_widget->pos());
            layout->removeWidget(m_widget);
            m_widget->setParent(m_container);
            d->m_floatingHelper->setFloating(true, Qt::Tool);
            asWindow(size, extraOffset);
            break;
        }
        case Window: {
            auto size = m_widget->size();
            auto extraOffset =
                m_viewMode == DockPinned ? QPoint{0, 0} : (m_widget->mapToGlobal({0, 0}) - m_widget->pos());
            layout->removeWidget(m_widget);
            m_widget->setParent(nullptr);
            d->m_floatingHelper->setFloating(false);
            m_widget->setWindowFlags(Qt::Window);
            asWindow(size, extraOffset);
            break;
        }
    }

    auto org = m_viewMode;
    m_viewMode = viewMode;

    // Tool Window extra
    {
        auto tw = qobject_cast<CDockToolWindow *>(m_widget);
        if (tw) {
            tw->viewModeChanged(viewMode);
        }
    }

    emit viewModeChanged(viewMode, org);
}

Qt::Edge CDockCard::edge() const {
    Q_D(const CDockCard);
    return d->m_tabBar->sideBar()->edge();
}

QM::Priority CDockCard::priority() const {
    Q_D(const CDockCard);
    auto tabBar = d->m_tabBar;
    return tabBar->sideBar()->firstBar() == tabBar ? QM::Primary : QM::Secondary;
}

void CDockCard::moveWidget(const QPoint &pos) {
    Q_D(CDockCard);
    auto &m_widget = d->m_widget;

    m_widget->move(pos);

    adjustWindowGeometry(m_widget);
}

void CDockCard::toggleMaximizeState() {
    Q_D(CDockCard);
    auto sideBar = d->m_tabBar->sideBar();
    auto frame = sideBar->frame();
    frame->toggleMaximize(sideBar->edge());
}

void CDockCard::setSize(const QSize &size) {
    if (size.isEmpty())
        return;

    Q_D(CDockCard);
    switch (d->m_viewMode) {
        case DockPinned: {
            auto sideBar = d->m_tabBar->sideBar();
            if (sideBar->orientation() == Qt::Horizontal) {
                sideBar->frame()->setEdgeSize(sideBar->edge(), size.height());
            } else {
                sideBar->frame()->setEdgeSize(sideBar->edge(), size.width());
            }
            break;
        }
        default:
            d->m_widget->resize(size);
            break;
    }
}

void CDockCard::mousePressEvent(QMouseEvent *event) {
    Q_D(CDockCard);
    auto &m_dragPos = d->m_dragPos;
    auto &m_readyDrag = d->m_readyDrag;

    CLTabButton::mousePressEvent(event);
    m_dragPos = event->pos();
    m_readyDrag = true;
}

void CDockCard::mouseMoveEvent(QMouseEvent *event) {
    Q_D(CDockCard);
    auto &m_dragPos = d->m_dragPos;
    auto &m_dragOffset = d->m_dragOffset;
    auto &m_readyDrag = d->m_readyDrag;

    CLTabButton::mouseMoveEvent(event);
    if (m_readyDrag) {
        QPoint pos = event->pos();
        if (qAbs(pos.x() - m_dragPos.x()) >= m_dragOffset.width() ||
            qAbs(pos.y() - m_dragPos.y()) >= m_dragOffset.height()) {
            m_readyDrag = false;

            if (auto tabBar = d->m_tabBar) {
                emit tabBar->dragStarted(this, m_dragPos, d->cardShot());
            }
        }
    }
}

void CDockCard::mouseReleaseEvent(QMouseEvent *event) {
    Q_D(CDockCard);
    auto &m_readyDrag = d->m_readyDrag;

    CLTabButton::mouseReleaseEvent(event);
    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CDockCard::leaveEvent(QEvent *event) {
    Q_D(CDockCard);

    Q_UNUSED(event)

    auto &m_readyDrag = d->m_readyDrag;

    if (m_readyDrag) {
        m_readyDrag = false;
    }
}

void CDockCard::paintEvent(QPaintEvent *event) {
    Q_D(CDockCard);
    return (d->m_orientation == Qt::Horizontal) ? CTabButton::paintEvent(event) : CLTabButton::paintEvent(event);
}

void CDockCard::contextMenuEvent(QContextMenuEvent *event) {
    Q_D(CDockCard);
    auto tw = qobject_cast<CDockToolWindow *>(d->m_widget);

    auto menu = tw ? tw->createCardMenu() : nullptr;
    if (!menu)
        menu = CDockCardPrivate::createViewModeMenu(this);

    menu->exec(QCursor::pos());
    menu->deleteLater();
}

bool CDockCard::eventFilter(QObject *obj, QEvent *event) {
    Q_D(CDockCard);
    if (obj == d->m_widget) {
        switch (event->type()) {
            case QEvent::Close: {
                d->m_closing = true;
                QTimer::singleShot(0, this, [d]() {
                    d->m_closing = false; //
                });
                break;
            }
            case QEvent::Hide: {
                if (d->m_closing) {
                    // Close accepted
                    setChecked(false);
                    if (d->m_viewMode == DockPinned) {
                        QTimer::singleShot(0, this, [d]() {
                            d->m_widget->show(); //
                        });
                    }
                }
                break;
            }
            case QEvent::Show:
            case QEvent::Move:
            case QEvent::Resize: {
                if (d->m_viewMode != DockPinned) {
                    d->oldGeometry = d->m_widget->geometry();
                }
                break;
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}

CDockCard::CDockCard(CDockCardPrivate &d, QWidget *parent) : CLTabButton(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}