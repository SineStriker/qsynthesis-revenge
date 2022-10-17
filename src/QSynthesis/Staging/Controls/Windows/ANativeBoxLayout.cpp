#include "ANativeBoxLayout.h"

#include <QDebug>

ANativeBoxLayout::ANativeBoxLayout(Direction dir, QWidget *parent) : QBoxLayout(dir, parent) {
}

ANativeBoxLayout::~ANativeBoxLayout() {
}

QWidget *ANativeBoxLayout::widgetAt(int index) const {
    return itemAt(index)->widget();
}

void ANativeBoxLayout::setWidgetAt(int index, QWidget *widget) {
    auto item = takeAt(index);
    QWidget *w = item->widget();
    if (w) {
        w->deleteLater();
    }
    delete item;
    if (!widget) {
        insertDefaultSpace(index);
    } else {
        insertWidget(index, widget);
    }
}

QWidget *ANativeBoxLayout::takeWidgetAt(int index) {
    auto item = itemAt(index);
    QWidget *w = item->widget();
    if (w) {
        item = takeAt(index);
        delete item;
        insertDefaultSpace(index);
    }
    return w;
}

void ANativeBoxLayout::insertDefaultSpace(int index) {
    insertSpacerItem(index, new QSpacerItem(0, 0));
}
