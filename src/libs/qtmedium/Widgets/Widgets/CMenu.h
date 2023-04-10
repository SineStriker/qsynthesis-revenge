#ifndef CMENU_H
#define CMENU_H

#include <QMenu>

#include "QMWidgetsGlobal.h"
#include "QSvgUri.h"

class CMenuPrivate;

class QMWIDGETS_API CMenu : public QMenu {
    Q_OBJECT
    Q_PROPERTY(QSvgUri subIcon READ subIcon WRITE setSubIcon NOTIFY styleChanged)
    Q_PROPERTY(QSvgUri subIconActive READ subIconActive WRITE setSubIconActive NOTIFY styleChanged)
    Q_PROPERTY(QSvgUri subIconDisabled READ subIconDisabled WRITE setSubIconDisabled NOTIFY styleChanged)
    Q_PROPERTY(QSize subIconMargins READ subIconMargins WRITE setSubIconMargins NOTIFY styleChanged)
public:
    explicit CMenu(QWidget *parent = nullptr);
    explicit CMenu(const QString &title, QWidget *parent = nullptr);
    ~CMenu();

public:
    QSvgUri subIcon() const;
    void setSubIcon(const QSvgUri &icon);

    QSvgUri subIconActive() const;
    void setSubIconActive(const QSvgUri &icon);

    QSvgUri subIconDisabled() const;
    void setSubIconDisabled(const QSvgUri &subIconDisabled);

    QSize subIconMargins() const;
    void setSubIconMargins(const QSize &margins);

protected:
    void paintEvent(QPaintEvent *event) override;
    void initStyleOption(QStyleOptionMenuItem *option, const QAction *action) const;

protected:
    CMenu(CMenuPrivate &d, QWidget *parent = nullptr);
    QScopedPointer<CMenuPrivate> d_ptr;

    Q_DECLARE_PRIVATE(CMenu)

signals:
    void styleChanged();
};

#endif // CMENU_H
