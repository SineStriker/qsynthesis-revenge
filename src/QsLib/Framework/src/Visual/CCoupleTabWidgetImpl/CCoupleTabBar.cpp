#include "CCoupleTabBar.h"
#include "CCoupleTabDoubleBar.h"

#include <QDrag>
#include <QHBoxLayout>
#include <QMimeData>
#include <QVBoxLayout>

#include "EventHelper.h"

using namespace CCoupleTabTypes;

CCoupleTabBar::CCoupleTabBar(QWidget *parent) : CCoupleTabBar(Forward, parent) {
}

CCoupleTabBar::CCoupleTabBar(BarDirection barDirection, QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground);

    m_placeholderIndex = -1;
    m_placeholderWidth = 0;

    m_barDirection = barDirection;
    m_cardDirection = TopToBottom;
    m_orientation = Qt::Horizontal;

    resetLayout();
}

CCoupleTabBar::~CCoupleTabBar() {
}

Qt::Orientation CCoupleTabBar::orientation() const {
    return m_orientation;
}

void CCoupleTabBar::setOrientation(Qt::Orientation orient) {
    if (m_orientation != orient) {
        m_orientation = orient;
        resetLayout();
    }
}

BarDirection CCoupleTabBar::barDirection() const {
    return m_barDirection;
}

void CCoupleTabBar::setBarDirection(const BarDirection &barDirection) {
    if (m_barDirection != barDirection) {
        m_barDirection = barDirection;
        resetLayout();
    }
}

CardDirection CCoupleTabBar::cardDirection() const {
    return m_cardDirection;
}

void CCoupleTabBar::setCardDirection(const CardDirection &cardDirection) {
    m_cardDirection = cardDirection;
    for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
        auto card = *it;
        card->setLongitudinalDirection(
            static_cast<CCoupleTabBarCard::LongitudinalDirection>(cardDirection));
    }
}

int CCoupleTabBar::placeholder() const {
    return m_placeholderIndex;
}

void CCoupleTabBar::setPlaceholder(int placeholder, int width) {
    removePlaceholder();

    QBoxLayout *layout = static_cast<QBoxLayout *>(this->layout());
    if (!layout) {
        return;
    }
    if (placeholder >= 0 && placeholder < layout->count()) {
        QWidget *w = createPlaceholder(width);
        layout->insertWidget(placeholder, w);
        m_placeholderIndex = placeholder;
        m_placeholderWidth = width;
    }
}

void CCoupleTabBar::removePlaceholder() {
    QBoxLayout *layout = static_cast<QBoxLayout *>(this->layout());
    if (!layout) {
        return;
    }
    if (m_placeholderIndex >= 0) {
        auto w = layout->itemAt(m_placeholderIndex)->widget();
        layout->removeWidget(w);
        delete w;
    }
    m_placeholderIndex = -1;
    m_placeholderWidth = 0;
}

void CCoupleTabBar::addCard(CCoupleTabBarCard *card) {
    insertCard(m_cards.size(), card);
}

void CCoupleTabBar::insertCard(int index, CCoupleTabBarCard *card) {
    if (!card->widget()) {
        qWarning().nospace() << "CCoupleTabBar: card \"" << card
                             << "\" doesn't have a valid widget pointer.";
        return;
    }

    QBoxLayout *layout = static_cast<QBoxLayout *>(this->layout());
    if (m_cards.contains(card)) {
        return;
    }
    resetCardTransform(card);

    // Uncheck all other cards
    if (card->isChecked()) {
        for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
            auto cur = *it;
            cur->blockSignals(true);
            cur->setChecked(false);
            cur->blockSignals(false);
        }
    }

    if (index >= m_cards.size()) {
        layout->addWidget(card);
        m_cards.append(card);
    } else if (index <= 0) {
        layout->insertWidget(0, card);
        m_cards.prepend(card);
    } else {
        layout->insertWidget(index, card);
        m_cards.insert(index, card);
    }
    card->show();
    connect(card, &CCoupleTabBarCard::startDrag, this, &CCoupleTabBar::handleStartDrag);
    connect(card, &CCoupleTabBarCard::toggled, this, &CCoupleTabBar::handleToggled);

    emit cardAdded(card);
}

void CCoupleTabBar::insertCard(CCoupleTabBarCard *indexCard, CCoupleTabBarCard *card) {
    return insertCard(indexOf(indexCard),card);
}

void CCoupleTabBar::removeCard(CCoupleTabBarCard *card) {
    QBoxLayout *layout = static_cast<QBoxLayout *>(this->layout());
    if (!m_cards.contains(card)) {
        return;
    }
    disconnect(card, &CCoupleTabBarCard::startDrag, this, &CCoupleTabBar::handleStartDrag);
    disconnect(card, &CCoupleTabBarCard::toggled, this, &CCoupleTabBar::handleToggled);
    m_cards.removeOne(card);
    layout->removeWidget(card);
    emit cardRemoved(card);
}

void CCoupleTabBar::removeAllCards() {
    QBoxLayout *layout = static_cast<QBoxLayout *>(this->layout());
    for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
        auto card = *it;
        disconnect(card, &CCoupleTabBarCard::startDrag, this, &CCoupleTabBar::handleStartDrag);
        disconnect(card, &CCoupleTabBarCard::toggled, this, &CCoupleTabBar::handleToggled);
        layout->removeWidget(card);
        emit cardRemoved(card);
    }
    m_cards.clear();
}

int CCoupleTabBar::count() const {
    return m_cards.size();
}

int CCoupleTabBar::indexOf(CCoupleTabBarCard *card) const {
    return m_cards.indexOf(card);
}

QList<CCoupleTabBarCard *> CCoupleTabBar::cards() const {
    return m_cards;
}

CCoupleTabBarCard *CCoupleTabBar::cardAt(int index) const {
    if (index < 0 || index >= m_cards.size()) {
        return nullptr;
    }
    return m_cards.at(index);
}

CCoupleTabBarCard *CCoupleTabBar::cardAtPos(QPoint pos) const {
    return qobject_cast<CCoupleTabBarCard *>(childAt(pos));
}

int CCoupleTabBar::activeIndex() const {
    int i = 0;
    int res = -1;
    for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
        auto card = *it;
        if (card->isChecked()) {
            res = i;
            break;
        }
        i++;
    }
    return res;
}

void CCoupleTabBar::setActiveIndex(int index) {
    if (index < 0 || index >= m_cards.size()) {
        int cur = activeIndex();
        if (cur >= 0) {
            m_cards.at(cur)->setChecked(false);
        }
        return;
    }
    auto card = m_cards.at(index);
    for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
        auto cur = *it;
        cur->blockSignals(true);
        cur->setChecked(cur == card);
        cur->blockSignals(false);
    }
    emit cardToggled(card);
}

CCoupleTabDoubleBar *CCoupleTabBar::doubleTabBar() const {
    return qobject_cast<CCoupleTabDoubleBar *>(parentWidget());
}

void CCoupleTabBar::resetLayout() {
    // Store org
    QList<QWidget *> widgets;
    QBoxLayout *layout = static_cast<QBoxLayout *>(this->layout());
    if (layout) {
        for (int i = 0; i < layout->count(); ++i) {
            auto w = layout->itemAt(i)->widget();
            widgets.append(w);
        }
        delete layout;
    }

    // New
    if (m_orientation == Qt::Horizontal) {
        layout = new QHBoxLayout();
        layout->setDirection((m_barDirection == Forward) ? QBoxLayout::LeftToRight
                                                         : QBoxLayout::RightToLeft);
        setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    } else {
        layout = new QVBoxLayout();
        layout->setDirection((m_barDirection == Forward) ? QBoxLayout::TopToBottom
                                                         : QBoxLayout::BottomToTop);
        setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    }

    layout->setMargin(0);
    layout->setSpacing(0);

    // Insert
    for (auto it = widgets.begin(); it != widgets.end(); ++it) {
        layout->addWidget(*it);
    }

    for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
        auto card = *it;
        resetCardTransform(card);
    }

    setLayout(layout);
}

void CCoupleTabBar::resetCardTransform(CCoupleTabBarCard *card) const {
    if (card->orientation() != m_orientation) {
        card->setOrientation(m_orientation);
    }
    auto ld = static_cast<CCoupleTabBarCard::LongitudinalDirection>(m_cardDirection);
    if (card->longitudinalDirection() != ld) {
        card->setLongitudinalDirection(ld);
    }
}

void CCoupleTabBar::handleStartDrag(const QPoint &pos, const QPixmap &pixmap) {
    auto card = qobject_cast<CCoupleTabBarCard *>(sender());
    emit dragStarted(card, pos, pixmap);
}

void CCoupleTabBar::handleToggled(bool checked) {
    auto card = qobject_cast<CCoupleTabBarCard *>(sender());
    if (checked) {
        setActiveIndex(indexOf(card));
    } else {
        emit cardToggled(card);
    }
}

QWidget *CCoupleTabBar::createPlaceholder(int width) const {
    QWidget *w = new QWidget();
    if (m_orientation == Qt::Horizontal) {
        w->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
        w->resize(width, w->height());
    } else {
        w->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
        w->resize(w->width(), width);
    }
    return w;
}

void CCoupleTabBar::paintEvent(QPaintEvent *event) {
    return QWidget::paintEvent(event);
}
