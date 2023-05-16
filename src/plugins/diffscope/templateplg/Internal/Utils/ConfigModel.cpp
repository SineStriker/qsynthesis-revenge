#include "ConfigModel.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
namespace TemplatePlg {
    namespace Internal {
        ConfigModel::ConfigModel(QObject *parent) : QStandardItemModel(parent) {
            this->setColumnCount(2);
            this->setHeaderData(0, Qt::Horizontal, "选项");
            this->setHeaderData(1, Qt::Horizontal, "值");
        }
        void ConfigModel::loadJsonFile(const QString &filePath) {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            }

            QByteArray jsonBytes = file.readAll();
            QJsonParseError error;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes, &error);
            if (error.error != QJsonParseError::NoError) {
            }

            QJsonObject jsonObj = jsonDoc.object();

            QStandardItem *rootItem = invisibleRootItem();
            populateModel(rootItem, jsonObj);
        }


        void ConfigModel::populateModel(QStandardItem *parentItem, const QJsonObject &jsonObject) {
            for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
                QString key = it.key();
                QJsonValue value = it.value();

                QStandardItem *item = new QStandardItem(key);

                if (value.isString()) {
                    QStandardItem *valueItem = new QStandardItem(value.toString());
                    QList<QStandardItem *> rowItems;
                    rowItems << item << valueItem;
                    parentItem->appendRow(rowItems);
                } else if (value.isBool()) {
                    QStandardItem *valueItem = new QStandardItem;
                    valueItem->setCheckable(true);
                    valueItem->setCheckState(value.toBool() ? Qt::Checked : Qt::Unchecked);
                    QList<QStandardItem *> rowItems;
                    rowItems << item << valueItem;
                    parentItem->appendRow(rowItems);
                } else if (value.isObject()) {
                    QJsonObject childObject = value.toObject();
                    populateModel(item, childObject);
                    parentItem->appendRow(item);
                } else if (value.isArray()) {
                    QJsonArray childArray = value.toArray();
                    for (int i = 0; i < childArray.size(); ++i) {
                        QJsonValue childValue = childArray.at(i);
                        if (childValue.isObject()) {
                            QJsonObject childObject = childValue.toObject();
                            QStandardItem *childItem = new QStandardItem;
                            populateModel(childItem, childObject);
                            item->appendRow(childItem);
                        } else {
                            QStandardItem *childItem = new QStandardItem(childValue.toString());
                            item->appendRow(childItem);
                        }
                    }
                    parentItem->appendRow(item);
                }
            }
        }
    }
}