#ifndef CHORUSKIT_CONFIGWIDGET_H
#define CHORUSKIT_CONFIGWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include "ConfigModel.h"

class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget *parent = nullptr);

private:
    QTreeView *m_treeView;
    ConfigModel *m_model;

private slots:
    void saveJson();
};




#endif // CHORUSKIT_CONFIGWIDGET_H
