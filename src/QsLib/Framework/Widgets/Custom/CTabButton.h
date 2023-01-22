#ifndef CTABBUTTON_H
#define CTABBUTTON_H

#include "CPushButton.h"
#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API CTabButton : public CPushButton {
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

protected:
    double m_spaceRatio;

    virtual QIcon paintedIcon() const;

    void paintEvent(QPaintEvent *event) override;

signals:
    void spaceChanged();
};

#endif // CTABBUTTON_H
