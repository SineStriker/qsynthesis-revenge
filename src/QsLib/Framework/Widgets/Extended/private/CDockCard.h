#ifndef __CDOCKCARD_H__
#define __CDOCKCARD_H__

#include "CLTabButton.h"

#include "QsFrameworkGlobal.h"
#include "QsMacros.h"

class CDockTabBar;

class QSFRAMEWORK_API CDockCard : public CLTabButton {
    Q_OBJECT
    Q_PROPERTY(QSize dragOffset READ dragOffset WRITE setDragOffset NOTIFY dragOffsetChanged)
public:
    explicit CDockCard(QWidget *parent = nullptr);
    explicit CDockCard(const QString &text, QWidget *parent = nullptr);
    CDockCard(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CDockCard();

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

    CDockTabBar *tabBar() const;

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

#endif // __CDOCKCARD_H__