#ifndef CTOOLBUTTON_H
#define CTOOLBUTTON_H

#include <QDebug>
#include <QToolButton>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_EXPORT CToolButton : public QToolButton {
    Q_OBJECT
public:
    explicit CToolButton(QWidget *parent = nullptr);
    explicit CToolButton(const QString &text, QWidget *parent = nullptr);
    CToolButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CToolButton();

protected:
    void paintEvent(QPaintEvent *event) override;

    QColor currentTextColor(const QSize &hint = {}) const;
};

#endif // CTOOLBUTTON_H
