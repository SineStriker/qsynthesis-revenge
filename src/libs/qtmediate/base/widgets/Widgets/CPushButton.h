#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_EXPORT CPushButton : public QPushButton {
    Q_OBJECT
public:
    explicit CPushButton(QWidget *parent = nullptr);
    explicit CPushButton(const QString &text, QWidget *parent = nullptr);
    CPushButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CPushButton();

protected:
    void paintEvent(QPaintEvent *event) override;

    QColor currentTextColor(const QSize &hint = {}) const;
};

#endif // CPUSHBUTTON_H
