#ifndef QMEQUALBOXLAYOUT_H
#define QMEQUALBOXLAYOUT_H

#include <QBoxLayout>

#include "QMWidgetsGlobal.h"

class QMEqualBoxLayoutPrivate;

class QMWIDGETS_EXPORT QMEqualBoxLayout : public QBoxLayout {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMEqualBoxLayout)
public:
    QMEqualBoxLayout(Direction direction, QWidget *parent = nullptr);
    ~QMEqualBoxLayout();

    void addWidgetE(QWidget *, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void insertWidgetE(int index, QWidget *widget, int stretch = 0,
                       Qt::Alignment alignment = Qt::Alignment());

    void addItem(QLayoutItem *item) override;
    QLayoutItem *takeAt(int index) override;

protected:
    QScopedPointer<QMEqualBoxLayoutPrivate> d_ptr;
};

#endif // QMEQUALBOXLAYOUT_H
