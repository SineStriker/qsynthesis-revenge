#include "JsonConfigWidget.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace TemplatePlg {
    namespace Internal {
        JsonConfigWidget::JsonConfigWidget(QWidget *parent) : QWidget(parent) {
            m_treeView = new QTreeView(this);
            m_model = new ConfigModel(this);
            m_treeView->setModel(m_model);

            m_model->loadJsonFile(":/configs/config.json");

            QHBoxLayout *buttonLayout = new QHBoxLayout;
            buttonLayout->addStretch();
            QPushButton *saveButton = new QPushButton(tr("Save"), this);
            connect(saveButton, &QPushButton::clicked, this, &JsonConfigWidget::saveJson);
            buttonLayout->addWidget(saveButton);

            QVBoxLayout *mainLayout = new QVBoxLayout(this);
            m_treeView->expandAll();
            mainLayout->addWidget(m_treeView);
            mainLayout->addLayout(buttonLayout);
            setLayout(mainLayout);
        }

        QJsonObject saveTreeViewToJson(QStandardItemModel *model, const QModelIndex &parentIndex = QModelIndex()) {
            QJsonObject resJsonObject;

            int rowCount = model->rowCount(parentIndex);
            for (int i = 0; i < rowCount; i++) {
                QModelIndex index = model->index(i, 0, parentIndex);
                QStandardItem *itemKey = model->itemFromIndex(index);
                QStandardItem *itemValue = model->itemFromIndex(model->index(i, 1, parentIndex));

                if (!itemKey) {
                    continue;
                }

                QString key = itemKey->text();
                if (model->hasChildren(index)) {
                    // 如果有子项，则递归处理子项
                    QJsonObject subJsonObject = saveTreeViewToJson(model, index);
                    resJsonObject[key] = subJsonObject;
                } else if (itemValue->data(Qt::CheckStateRole).isValid()) {
                    QVariant value = itemValue->data(Qt::CheckStateRole);
                    resJsonObject[key] = value.toBool();
                } else {
                    QVariant value = itemValue->data(Qt::DisplayRole);
                    resJsonObject[key] = value.toString();
                }
            }
            return resJsonObject;
        }

        void JsonConfigWidget::saveJson() {
            QJsonObject jsonObject = saveTreeViewToJson(m_model);
            QFile file("./out.json");
            if (file.open(QIODevice::WriteOnly)) {
                QTextStream stream(&file);
                stream << QJsonDocument(jsonObject).toJson(QJsonDocument::Indented);
                file.close();
            }
        }
    }
}