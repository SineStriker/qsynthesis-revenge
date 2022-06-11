#ifndef CCOUPLETABBAR_H
#define CCOUPLETABBAR_H

#include <QWidget>

#include "CCoupleTabBarCard.h"

class CCoupleTabDoubleBar;

class CCoupleTabBar : public QWidget {
    Q_OBJECT
public:
    explicit CCoupleTabBar(QWidget *parent = nullptr);
    explicit CCoupleTabBar(CCoupleTabTypes::BarDirection barDirection, QWidget *parent = nullptr);
    ~CCoupleTabBar();

public:
    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orient);

    CCoupleTabTypes::BarDirection barDirection() const;
    void setBarDirection(const CCoupleTabTypes::BarDirection &barDirection);

    CCoupleTabTypes::CardDirection cardDirection() const;
    void setCardDirection(const CCoupleTabTypes::CardDirection &cardDirection);

    int placeholder() const;
    void setPlaceholder(int placeholder, int width);
    void removePlaceholder();

    void addCard(CCoupleTabBarCard *card);
    void insertCard(int index, CCoupleTabBarCard *card);
    void insertCard(CCoupleTabBarCard *indexCard, CCoupleTabBarCard *card);
    void removeCard(CCoupleTabBarCard *card);
    void removeAllCards();

    int count() const;
    int indexOf(CCoupleTabBarCard *card) const;
    QList<CCoupleTabBarCard *> cards() const;
    CCoupleTabBarCard *cardAt(int index) const;

    CCoupleTabBarCard *cardAtPos(QPoint pos) const;

    int indexAtPos(QPoint pos) const;

    int activeIndex() const;
    void setActiveIndex(int index);

    CCoupleTabDoubleBar *doubleTabBar() const;

protected:
    void resetLayout();
    void resetCardTransform(CCoupleTabBarCard *card) const;

private:
    void handleStartDrag(const QPoint &pos, const QPixmap &pixmap);
    void handleToggled(bool checked);

protected:
    int m_placeholderIndex;
    int m_placeholderWidth;

    Qt::Orientation m_orientation;
    CCoupleTabTypes::BarDirection m_barDirection;
    CCoupleTabTypes::CardDirection m_cardDirection;

    QList<CCoupleTabBarCard *> m_cards;

    QWidget *createPlaceholder(int width) const;

    void paintEvent(QPaintEvent *event) override;

signals:
    void dragStarted(CCoupleTabBarCard *card, const QPoint &pos, const QPixmap &pixmap);

    void cardAdded(CCoupleTabBarCard *card);
    void cardRemoved(CCoupleTabBarCard *card);
    void cardToggled(CCoupleTabBarCard *card);
};

#endif // CCOUPLETABBAR_H
