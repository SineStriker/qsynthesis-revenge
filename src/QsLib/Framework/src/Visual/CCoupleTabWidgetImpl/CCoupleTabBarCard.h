#ifndef CCOUPLETABBARCARD_H
#define CCOUPLETABBARCARD_H

#include "CCoupleTabTypes.h"
#include "CLTabButton.h"
#include "qsframework_global.h"
#include "qsutils_macros.h"

#include <QSizePolicy>

class CCoupleTabBar;

class QSFRAMEWORK_API CCoupleTabBarCard : public CLTabButton {
    Q_OBJECT
    Q_UNIQUE_ID(CCoupleTabBarCard)
    Q_PROPERTY(QSize dragOffset READ dragOffset WRITE setDragOffset NOTIFY dragOffsetChanged)
public:
    explicit CCoupleTabBarCard(QWidget *parent = nullptr);
    explicit CCoupleTabBarCard(const QString &text, QWidget *parent = nullptr);
    CCoupleTabBarCard(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CCoupleTabBarCard();

private:
    void init();

public:
    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orient);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    int widthHint() const;

    QSize dragOffset() const;
    void setDragOffset(const QSize &dragOffset);

    QPixmap cardShot() const;

    CCoupleTabBar *tabBar() const;

    QWidget *widget() const;
    void setWidget(QWidget *widget);

protected:
    Qt::Orientation m_orientation;
    QSizePolicy m_sizePolicyH, m_sizePolicyV;

    QPoint m_dragPos;
    QSize m_dragOffset;

    bool m_readyDrag;

    QWidget *m_widget;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void startDrag(const QPoint &pos, const QPixmap &pixmap);
    void dragOffsetChanged();
};

#endif // CCOUPLETABBARCARD_H
