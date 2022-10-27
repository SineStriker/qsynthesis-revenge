#ifndef VSPEAKERTREEWIDGET_H
#define VSPEAKERTREEWIDGET_H

#include <QTreeWidget>

class VSpeakerTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    explicit VSpeakerTreeWidget(QWidget *parent = nullptr);
    ~VSpeakerTreeWidget();

signals:
};

#endif // VSPEAKERTREEWIDGET_H
