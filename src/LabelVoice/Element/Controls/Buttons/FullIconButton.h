#ifndef FULLICONBUTTON_H
#define FULLICONBUTTON_H

#include "CPushButton.h"

#include "lvelem_global.h"

class LVELEM_API FullIconButton : public CPushButton {
    Q_OBJECT
    Q_PROPERTY(QSize iconMargin READ iconMargin WRITE setIconMargin NOTIFY styleChanged)
public:
    explicit FullIconButton(QWidget *parent = nullptr);
    ~FullIconButton();

public:
    QSize iconMargin() const;
    void setIconMargin(const QSize &iconMargin);

protected:
    void paintEvent(QPaintEvent *event) override;

    QSize m_iconMargin;

signals:
    void styleChanged();
};

#endif // FULLICONBUTTON_H
