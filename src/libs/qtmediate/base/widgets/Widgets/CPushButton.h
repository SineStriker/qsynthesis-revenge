#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QDebug>
#include <QPushButton>

#include "QMWidgetsGlobal.h"
#include "QSvgIconEx.h"

class QMWIDGETS_EXPORT CPushButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(QSvgIconEx svgIcon READ svgIcon WRITE setSvgIcon)
public:
    explicit CPushButton(QWidget *parent = nullptr);
    explicit CPushButton(const QString &text, QWidget *parent = nullptr);
    CPushButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CPushButton();

public:
    QSvgIconEx svgIcon() const;
    void setSvgIcon(const QSvgIconEx &icon);

protected:
    void paintEvent(QPaintEvent *event) override;

    QColor currentTextColor(const QSize &hint = {}) const;
};

#endif // CPUSHBUTTON_H
