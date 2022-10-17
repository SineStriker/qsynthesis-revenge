#ifndef CCOUPLETABDOUBLEBAR_H
#define CCOUPLETABDOUBLEBAR_H

#include <QWidget>

#include "CCoupleTabBar.h"
#include "qsframework_global.h"

class CCoupleTabDragProxy;

class QSFRAMEWORK_API CCoupleTabDoubleBar : public QWidget {
    Q_OBJECT
public:
    explicit CCoupleTabDoubleBar(QWidget *parent = nullptr);
    ~CCoupleTabDoubleBar();

    friend class CCoupleTabBar;

private:
    void init();

public:
    CCoupleTabTypes::CardDirection cardDirection() const;
    void setCardDirection(CCoupleTabTypes::CardDirection cardDirection);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orient);

    bool highlight() const;
    void setHighlight(bool highlight, int widthHint = 0);

    int count() const;

    CCoupleTabBar *firstBar() const;
    CCoupleTabBar *secondBar() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    CCoupleTabBar *m_firstBar;
    CCoupleTabBar *m_secondBar;

    void resetLayout();

    void mousePressEvent(QMouseEvent *event) override;

private:
    void handleCardAdded(CCoupleTabBarCard *card);
    void handleCardRemoved(CCoupleTabBarCard *card);
    void handleCardToggled(CCoupleTabBarCard *card);

signals:
    void cardAdded(CCoupleTabTypes::Number number, CCoupleTabBarCard *card);
    void cardRemoved(CCoupleTabTypes::Number number, CCoupleTabBarCard *card);
    void cardToggled(CCoupleTabTypes::Number number, CCoupleTabBarCard *card);
};

#endif // CCOUPLETABDOUBLEBAR_H
