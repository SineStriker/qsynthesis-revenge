#ifndef CCHECKBOX_H
#define CCHECKBOX_H

#include <QAbstractButton>
#include <QHBoxLayout>
#include <QLabel>

#include "CToolButton.h"
#include "QPixelSize.h"

#include "QMWidgetsGlobal.h"

class CCheckBoxPrivate;

class QMWIDGETS_EXPORT CCheckBox : public QAbstractButton {
    Q_OBJECT
    Q_LAYOUT_PROPERTY_DELCARE
public:
    explicit CCheckBox(QWidget *parent = nullptr);
    CCheckBox(const QString &text, QWidget *parent = nullptr);
    ~CCheckBox();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CCheckBoxPrivate *d;
};

#endif // CCHECKBOX_H
