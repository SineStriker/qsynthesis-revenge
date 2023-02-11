#ifndef CEQUALBOXLAYOUT_H
#define CEQUALBOXLAYOUT_H

#include <QBoxLayout>

#include "QsFrameworkGlobal.h"

class CEqualBoxLayoutPrivate;

class QSFRAMEWORK_API CEqualBoxLayout : public QBoxLayout {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CEqualBoxLayout)
public:
    CEqualBoxLayout(Direction direction, QWidget *parent = nullptr);
    ~CEqualBoxLayout();

    void addWidgetE(QWidget *, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
    void insertWidgetE(int index, QWidget *widget, int stretch = 0,
                       Qt::Alignment alignment = Qt::Alignment());

    void addItem(QLayoutItem *item) override;
    QLayoutItem *takeAt(int index) override;

protected:
    QScopedPointer<CEqualBoxLayoutPrivate> d_ptr;
};

#endif // CEQUALBOXLAYOUT_H
