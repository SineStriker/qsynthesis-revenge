#ifndef CLTABBUTTON_H
#define CLTABBUTTON_H

#include "CTabButton.h"
#include "QMWidgetsGlobal.h"

class QMWIDGETS_EXPORT CLTabButton : public CTabButton {
    Q_OBJECT
public:
    explicit CLTabButton(QWidget *parent = nullptr);
    explicit CLTabButton(const QString &text, QWidget *parent = nullptr);
    CLTabButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CLTabButton();

private:
    void init();

public:
    enum LongitudinalDirection { TopToBottom, BottomToTop };

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    LongitudinalDirection longitudinalDirection() const;
    void setLongitudinalDirection(LongitudinalDirection d);

protected:
    LongitudinalDirection m_longitudinalDirection;

    void paintEvent(QPaintEvent *event) override;
};

#endif // CLTABBUTTON_H
