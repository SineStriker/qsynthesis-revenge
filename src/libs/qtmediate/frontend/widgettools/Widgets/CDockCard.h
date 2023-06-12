#ifndef CDOCKCARD_H
#define CDOCKCARD_H

#include "CLTabButton.h"

#include "QMWidgetToolsGlobal.h"

class CDockTabBar;

class CDockCardPrivate;

class QMWTOOLS_EXPORT CDockCard : public CLTabButton {
    Q_OBJECT
    Q_PROPERTY(QSize dragOffset READ dragOffset WRITE setDragOffset)
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

public:
    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orient);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    int widthHint() const;

    QSize dragOffset() const;
    void setDragOffset(const QSize &dragOffset);

    QWidget *widget() const;
    QWidget *container() const;
    QWidget *takeWidget();
    void setWidget(QWidget *widget);

    ViewMode viewMode() const;
    void setViewMode(ViewMode viewMode);

    inline bool dockVisible() const {
        return isChecked() && viewMode() == DockPinned;
    }

    CDockTabBar *tabBar() const;
    void moveWidget(const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void viewModeChanged(ViewMode viewMode, ViewMode oldViewMode);

private:
    CDockCardPrivate *d;

    friend class CDockTabBar;
};

#endif // CDOCKCARD_H
