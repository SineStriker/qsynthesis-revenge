#ifndef CTOOLBUTTON_H
#define CTOOLBUTTON_H

#include <QDebug>
#include <QToolButton>

#include "QMWidgetsGlobal.h"
#include "QSvgIconEx.h"

class QMWIDGETS_EXPORT CToolButton : public QToolButton {
    Q_OBJECT
    Q_PROPERTY(QSvgIconEx svgIcon READ svgIcon WRITE setSvgIcon)
public:
    explicit CToolButton(QWidget *parent = nullptr);
    explicit CToolButton(const QString &text, QWidget *parent = nullptr);
    CToolButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CToolButton();

public:
    QSvgIconEx svgIcon() const;
    void setSvgIcon(const QSvgIconEx &icon);

protected:
    void paintEvent(QPaintEvent *event) override;

    QColor currentTextColor(const QSize &hint = {}) const;
};

#endif // CTOOLBUTTON_H
