#ifndef CHORUSKIT_CTOOLBAR_H
#define CHORUSKIT_CTOOLBAR_H

#include <QAction>
#include <QFrame>

#include "QMWidgetsGlobal.h"

class CToolBarPrivate;

class QMWIDGETS_API CToolBar : public QFrame {
    Q_OBJECT
public:
    explicit CToolBar(QWidget *parent = nullptr);
    ~CToolBar();

public:
    void clear();

    QAction *addSeparator();
    QAction *insertSeparator(QAction *before);

    QAction *addWidget(QWidget *widget);
    QAction *insertWidget(QAction *before, QWidget *widget);

    QSize iconSize() const;
    void setIconSize(const QSize &iconSize);

signals:
    void actionTriggered(QAction *action);
    void iconSizeChanged(const QSize &iconSize);

protected:
    void actionEvent(QActionEvent *event) override;

private:
    CToolBarPrivate *d;
};

#endif // CHORUSKIT_CTOOLBAR_H
