#ifndef CTABBUTTON_H
#define CTABBUTTON_H

#include "CPushButton.h"
#include "QMWidgetsGlobal.h"

class QMWIDGETS_EXPORT CTabButton : public CPushButton {
    Q_OBJECT
    Q_PROPERTY(double spaceRatio READ spaceRatio WRITE setSpaceRatio NOTIFY spaceChanged)
public:
    explicit CTabButton(QWidget *parent = nullptr);
    explicit CTabButton(const QString &text, QWidget *parent = nullptr);
    CTabButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CTabButton();

private:
    void init();

public:
    double spaceRatio() const;
    void setSpaceRatio(double ratio);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void spaceChanged();

protected:
    double m_spaceRatio;

    void paintEvent(QPaintEvent *event) override;
};

#endif // CTABBUTTON_H
