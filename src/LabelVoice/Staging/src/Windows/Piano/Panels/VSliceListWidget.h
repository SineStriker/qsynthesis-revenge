#ifndef VSLICELISTWIDGET_H
#define VSLICELISTWIDGET_H

#include <QListWidget>

class VSliceListWidget : public QListWidget {
    Q_OBJECT
public:
    explicit VSliceListWidget(QWidget *parent = nullptr);
    ~VSliceListWidget();

signals:
};

#endif // VSLICELISTWIDGET_H
