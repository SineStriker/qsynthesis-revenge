#ifndef CBASETITLEBARV2_H
#define CBASETITLEBARV2_H

#include <QWidget>

#include "ANativeTitleBar.h"

class CBaseTitleBarV2 : public ANativeTitleBar {
    Q_OBJECT
    Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor NOTIFY titleColorChanged)
public:
    explicit CBaseTitleBarV2(QWidget *parent = nullptr);
    ~CBaseTitleBarV2();

public:
    QColor titleColor() const;
    void setTitleColor(const QColor &titleColor);

protected:
    QColor m_titleColor;

signals:
    void titleColorChanged();
};

#endif // CBASETITLEBARV2_H
