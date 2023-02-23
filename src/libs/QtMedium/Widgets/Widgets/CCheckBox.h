#ifndef CCHECKBOX_H
#define CCHECKBOX_H

#include <QAbstractButton>
#include <QHBoxLayout>
#include <QLabel>

#include "CToolButton.h"
#include "QPixelSize.h"

#include "QMWidgetsGlobal.h"

class QMWIDGETS_API CCheckBox : public QAbstractButton {
    Q_OBJECT
    Q_LAYOUT_PROPERTY_DELCARE
public:
    explicit CCheckBox(QWidget *parent = nullptr);
    CCheckBox(const QString &text, QWidget *parent = nullptr);
    ~CCheckBox();

private:
    void init();

protected:
    CToolButton *m_box;
    QLabel *m_label;

    QHBoxLayout *m_layout;

private:
    void handleBoxToggled(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void checkStateSet() override;
    void nextCheckState() override;
};

#endif // CCHECKBOX_H
