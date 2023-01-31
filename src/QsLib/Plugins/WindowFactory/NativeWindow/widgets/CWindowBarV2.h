#ifndef CWINDOWBARV2_H
#define CWINDOWBARV2_H

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QWidget>

#include "CBaseTitleBarV2.h"
#include "CToolButton.h"
#include "QPixelSize.h"

class Q_DECL_EXPORT CWindowBarV2 : public CBaseTitleBarV2 {
    Q_OBJECT
    Q_PROPERTY(
        QPixelSize titleMargin READ titleMargin WRITE setTitleMargin NOTIFY styleChanged)
public:
    explicit CWindowBarV2(QMenuBar *menuBar, QWidget *parent = nullptr);
    ~CWindowBarV2();

    void reloadStrings();

public:
    void reloadMaxButtonState(bool checked = false);

    QPixelSize titleMargin() const;
    void setTitleMargin(const QPixelSize &titleMargin);

    bool titleVisible() const;
    void setTitleVisible(bool titleVisible);

private:
    CToolButton *m_minButton;
    CToolButton *m_maxButton;
    CToolButton *m_closeButton;
    CToolButton *m_iconButton;

    QLabel *m_titleLabel;

    QPixelSize m_titleMargin;
    bool m_titleVisible;

    QMenuBar *m_menuBar;

    void drawCentralTitle(QPainter &painter);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void minRequested();
    void maxRequested();
    void closeRequested();

    void styleChanged();

    friend class NativeHandlePrivate;
};

#endif // CWINDOWBARV2_H
