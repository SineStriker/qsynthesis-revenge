#ifndef CMENU_H
#define CMENU_H

#include <QMenu>

#include "QMWidgetsGlobal.h"
#include "QMarginsImpl.h"
#include "QSvgIconEx.h"

class CMenuPrivate;

class QMWIDGETS_EXPORT CMenu : public QMenu {
    Q_OBJECT
    Q_PROPERTY(QSvgIconEx rightArrowIcon READ rightArrowIcon WRITE setRightArrowIcon)
    Q_PROPERTY(QMargins rightArrowMargins READ rightArrowMargins WRITE setRightArrowMargins)
public:
    explicit CMenu(QWidget *parent = nullptr);
    explicit CMenu(const QString &title, QWidget *parent = nullptr);
    ~CMenu();

public:
    QSvgIconEx rightArrowIcon() const;
    void setRightArrowIcon(const QSvgIconEx &icon);

    QMargins rightArrowMargins() const;
    void setRightArrowMargins(const QMargins &margins);

protected:
    void paintEvent(QPaintEvent *event) override;

    void initStyleOption(QStyleOptionMenuItem *option, const QAction *action) const;

private:
    CMenuPrivate *d;
};

#endif // CMENU_H
