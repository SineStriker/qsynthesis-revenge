#include "CDockCard_p.h"

#include <QAction>
#include <QMenu>

CDockCardPrivate::CDockCardPrivate() {
    m_closing = false;

    m_container = nullptr;
    m_widget = nullptr;
    m_viewMode = CDockCard::DockPinned;

    m_dragOffset = QSize(10, 10);
    m_readyDrag = false;

    m_tabBar = nullptr;
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
    q->setContextMenuPolicy(Qt::CustomContextMenu);
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
    auto menu = new QMenu(card);

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