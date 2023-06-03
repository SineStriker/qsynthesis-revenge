#ifndef __CDOCKCARD_H__
#define __CDOCKCARD_H__

#include "CLTabButton.h"

#include "QMWidgetsGlobal.h"

class CDockTabBar;

class QMWIDGETS_API CDockCard : public CLTabButton {
    Q_OBJECT
    Q_PROPERTY(QSize dragOffset READ dragOffset WRITE setDragOffset NOTIFY dragOffsetChanged)
public:
    explicit CDockCard(QWidget *parent = nullptr);
    explicit CDockCard(const QString &text, QWidget *parent = nullptr);
    CDockCard(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CDockCard();

    enum ViewMode {
        DockPinned,
        Float,
        Window,
    };

    Q_ENUM(ViewMode)

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
    QWidget *container() const;
    QWidget *takeWidget();
    void setWidget(QWidget *widget);

    ViewMode viewMode() const;
    void setViewMode(ViewMode viewMode);

    inline bool dockVisible() const {
        return isChecked() && m_viewMode == DockPinned;
    }

    void moveWidget(const QPoint &pos);

protected:
    Qt::Orientation m_orientation;
    QSizePolicy m_sizePolicyH, m_sizePolicyV;

    QPoint m_dragPos;
    QSize m_dragOffset;

    bool m_readyDrag;

    QWidget *m_container;
    QWidget *m_widget;

    ViewMode m_viewMode;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void startDrag(const QPoint &pos, const QPixmap &pixmap);
    void dragOffsetChanged();
    void viewModeChanged(ViewMode viewMode, ViewMode oldViewMode);
};

#endif // __CDOCKCARD_H__