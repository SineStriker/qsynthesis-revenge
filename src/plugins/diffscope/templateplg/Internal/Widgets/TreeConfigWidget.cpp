#include "TreeConfigWidget.h"
#include <QFile>
#include <QJsonDocument>
#include <QListView>
#include <QLocale>
#include <QMessageBox>
#include <QVBoxLayout>

namespace TemplatePlg {
    namespace Internal {
        TreeConfigWidget::TreeConfigWidget(QString configPath, bool configGen, QWidget *parent)
            : configPath(configPath), configGen(configGen), QWidget(parent) {
            mainLayout = new QVBoxLayout(this);

            auto treeLayout = new QHBoxLayout(this);
            m_treeWidget = new QTreeWidget(this);
            m_treeWidget->setColumnCount(3);
            m_treeWidget->setHeaderLabels({"中文名", "EnName", "值"});
            treeLayout->addWidget(m_treeWidget);

            if (configGen) {
                auto buttonLayout = new QVBoxLayout(this);

                // zh name
                auto nameLayout = new QHBoxLayout();
                auto m_name_label = new QLabel("name", this);
                m_name = new QLineEdit(this);
                nameLayout->addWidget(m_name_label);
                nameLayout->addWidget(m_name);
                buttonLayout->addLayout(nameLayout);

                // en name
                auto ennameLayout = new QHBoxLayout();
                auto m_enname_label = new QLabel("en_name", this);
                m_enname = new QLineEdit(this);
                ennameLayout->addWidget(m_enname_label);
                ennameLayout->addWidget(m_enname);
                buttonLayout->addLayout(ennameLayout);

                // value
                auto valueLayout = new QHBoxLayout();
                auto m_value_label = new QLabel("value", this);
                m_value = new QLineEdit(this);
                valueLayout->addWidget(m_value_label);
                valueLayout->addWidget(m_value);
                buttonLayout->addLayout(valueLayout);

                // childType
                auto childLayout = new QHBoxLayout();
                auto m_child_label = new QLabel("childType:", this);
                m_childType = new QComboBox(this);
                m_childType->addItem("Top");
                m_childType->addItem("Child");
                childLayout->addWidget(m_child_label);
                childLayout->addWidget(m_childType);
                buttonLayout->addLayout(childLayout);

                // controlType
                auto controlLayout = new QHBoxLayout();
                auto m_control_label = new QLabel("controlType:", this);
                m_type = new QComboBox(this);
                m_type->addItem("Group");
                m_type->addItem("QCheckBox");
                m_type->addItem("QComboBox");
                m_type->addItem("QLineEdit");
                controlLayout->addWidget(m_control_label);
                controlLayout->addWidget(m_type);
                buttonLayout->addLayout(controlLayout);

                m_addButton = new QPushButton("add", this);
                m_removeButton = new QPushButton("remove", this);
                m_saveButton = new QPushButton("save", this);

                buttonLayout->addWidget(m_addButton);
                buttonLayout->addWidget(m_removeButton);
                buttonLayout->addWidget(m_saveButton);
                treeLayout->addLayout(buttonLayout);

                connect(m_addButton, &QPushButton::clicked, this, &TreeConfigWidget::addTableRow);
                connect(m_removeButton, &QPushButton::clicked, this, &TreeConfigWidget::removeTableRow);
                mainLayout->addLayout(treeLayout);
            } else {
                if (getLocalLanguage() == "Chinese") {
                    m_treeWidget->hideColumn(1);
                } else {
                    m_treeWidget->hideColumn(0);
                }
                m_saveButton = new QPushButton("save", this);
                mainLayout->addLayout(treeLayout);
                mainLayout->addWidget(m_saveButton);
            }

            connect(m_saveButton, &QPushButton::clicked, this, &TreeConfigWidget::saveConfig);
            loadConfig(readJsonFile());
            m_treeWidget->expandAll();
            this->setLayout(mainLayout);
        }

        TreeConfigWidget::~TreeConfigWidget() {
        }

        void TreeConfigWidget::addTableRow() {
            auto itemName = m_name->text();
            auto itemEnName = m_enname->text();
            auto value = m_value->text();


            auto *Label = new QLabel();
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList() << itemName << itemEnName);
            m_treeWidget->setItemWidget(child, 2, Label);

            QTreeWidgetItem *selectedItem = m_treeWidget->currentItem();
            if (!m_childType->currentIndex()) {
                qDebug() << "add Top:" << itemName;
                m_treeWidget->addTopLevelItem(child);
            } else {
                qDebug() << "add Child:" << itemName;
                selectedItem->addChild(child);
            }

            switch (m_type->currentIndex()) {
                case 0: {
                    auto count = m_treeWidget->topLevelItemCount();
                    break;
                }
                case 1: {
                    auto *frame = new QCheckBox();
                    m_treeWidget->setItemWidget(child, 2, frame);
                    break;
                }
                case 2: {
                    auto *frame = new QComboBox();
                    auto cbList = value.split(";");
                    if (cbList.size() % 2 == 0) {
                        for (auto it : cbList) {
                            frame->addItem(it);
                        }
                        m_treeWidget->setItemWidget(child, 2, frame);
                    } else {
                        QMessageBox mb;
                        mb.setText(
                            "Please input\r\n\"Chinese; Chinese; English; English\" in the following format to ensure "
                            "that the items are even numbers, and click addButton again.");
                        mb.exec();
                    }
                    break;
                }
                case 3: {
                    auto *frame = new QLineEdit();
                    frame->setText(value);
                    m_treeWidget->setItemWidget(child, 2, frame);
                    break;
                }
                default:
                    break;
            }
            m_treeWidget->expandAll();
            m_treeWidget->resizeColumnToContents(0);
            m_treeWidget->resizeColumnToContents(1);
        }

        void TreeConfigWidget::removeTableRow() {
            QTreeWidgetItem *selectedItem = m_treeWidget->currentItem();
            if (selectedItem != nullptr) {
                QTreeWidgetItem *parentItem = selectedItem->parent();
                if (parentItem != nullptr) {
                    parentItem->removeChild(selectedItem);
                } else {
                    m_treeWidget->takeTopLevelItem(m_treeWidget->indexOfTopLevelItem(selectedItem));
                }
                delete selectedItem;
            }
        }

        QJsonObject TreeConfigWidget::createJsonFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item) {
            QJsonObject json;

            // 如果item为空，则从根节点开始遍历
            if (!item) {
                item = treeWidget->invisibleRootItem();
            }

            // 遍历当前节点的每一个子节点
            for (int i = 0; i < item->childCount(); i++) {
                QTreeWidgetItem *childItem = item->child(i);
                QString type;
                auto itemWidget = m_treeWidget->itemWidget(childItem, 2);

                QJsonObject childJson;
                childJson["zh"] = childItem->text(0);
                childJson["en"] = childItem->text(1);

                if (QCheckBox *checkBox = qobject_cast<QCheckBox *>(itemWidget)) {
                    type = "QCheckBox";
                    childJson["value"] = qobject_cast<QCheckBox *>(itemWidget)->isChecked();
                } else if (QComboBox *comboBox = qobject_cast<QComboBox *>(itemWidget)) {
                    type = "QComboBox";
                    auto cb = qobject_cast<QComboBox *>(itemWidget);
                    QStringList cbList;
                    int count = cb->count();
                    for (int cb_i = 0; cb_i < count / 2; cb_i++) {
                        cbList.append(cb->itemText(cb_i));
                    }
                    childJson["zh_value"] = QJsonArray::fromStringList(cbList);
                    cbList.clear();
                    for (int cb_i = count / 2; cb_i < count; cb_i++) {
                        cbList.append(cb->itemText(cb_i));
                    }
                    childJson["en_value"] = QJsonArray::fromStringList(cbList);
                    childJson["index"] =
                        cb->currentIndex() < count / 2 ? cb->currentIndex() : cb->currentIndex() - (count / 2);
                } else {
                    type = "Group";
                }

                qDebug() << type;
                childJson["type"] = type;
                // 如果当前节点是一个组，则递归遍历它的子节点
                if (type == "Group" && childItem->childCount()) {
                    childJson["value"] = createJsonFromTree(treeWidget, childItem);
                    json[QString::number(i)] = childJson;
                } else {
                    json[QString::number(i)] = childJson;
                }
            }

            return json;
        }

        QJsonObject TreeConfigWidget::readJsonFile() {
            QFile file(configPath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                // Failed to open file
                return QJsonObject();
            }

            QByteArray jsonData = file.readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

            if (jsonDoc.isNull() || !jsonDoc.isObject()) {
                // Failed to parse JSON
                return QJsonObject();
            }

            return jsonDoc.object();
        }

        void TreeConfigWidget::loadConfig(const QJsonObject &config, QTreeWidgetItem *parent) {
            for (auto it = config.begin(); it != config.end(); ++it) {
                const QString &key = it.key();
                const QJsonValue &value = it.value();

                QTreeWidgetItem *item = new QTreeWidgetItem(parent);

                // Set zh and en columns
                item->setText(0, value.toObject().value("zh").toString());
                item->setText(1, value.toObject().value("en").toString());

                QString type = value.toObject().value("type").toString();
                if (type == "Group") {
                    // Insert Qlabel for Group type
                    QLabel *label = new QLabel(m_treeWidget);
                    m_treeWidget->setItemWidget(item, 2, label);
                } else if (type == "QComboBox") {
                    // Insert QComboBox for QComboBox type
                    QComboBox *comboBox = new QComboBox(m_treeWidget);
                    int index = value.toObject().value("index").toInt();

                    QStringList zhValues = value.toObject().value("zh_value").toVariant().toStringList();
                    QStringList enValues = value.toObject().value("en_value").toVariant().toStringList();
                    QListView *view = qobject_cast<QListView *>(comboBox->view());
                    comboBox->addItems(zhValues);
                    comboBox->addItems(enValues);
                    if (!configGen && getLocalLanguage() == "Chinese") {
                        for (int c_i = 0; c_i < zhValues.size(); c_i++) {
                            view->setRowHidden(c_i + zhValues.size(), true);
                        }
                        index = index < zhValues.size() ? index : index - zhValues.size();
                    } else if (!configGen && getLocalLanguage() == "English") {
                        for (int c_i = 0; c_i < zhValues.size(); c_i++) {
                            view->setRowHidden(c_i, true);
                        }
                        index = index > zhValues.size() ? index : index + zhValues.size();
                    }
                    comboBox->setCurrentIndex(index);
                    m_treeWidget->setItemWidget(item, 2, comboBox);
                } else if (type == "QCheckBox") {
                    // Insert QCheckBox for QCheckBox type
                    QCheckBox *checkBox = new QCheckBox(m_treeWidget);
                    checkBox->setChecked(value.toObject().value("value").toBool());
                    m_treeWidget->setItemWidget(item, 2, checkBox);
                } else if (type == "QLineEdit") {
                    // Insert QLineEdit for QLineEdit type
                    QLineEdit *lineEdit = new QLineEdit(m_treeWidget);
                    lineEdit->setText(value.toObject().value("value").toString());
                    m_treeWidget->setItemWidget(item, 2, lineEdit);
                }

                if (parent) {
                    parent->addChild(item);
                } else {
                    m_treeWidget->addTopLevelItem(item);
                }

                if (type == "Group") {
                    // Recursively load child items
                    loadConfig(value["value"].toObject(), item);
                }
            }
        }

        bool TreeConfigWidget::saveConfig() {
            QJsonObject configJson = createJsonFromTree(m_treeWidget);
            QFile file(configPath);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                // Failed to open file
                return false;
            }

            QJsonDocument jsonDoc(configJson);
            file.write(jsonDoc.toJson());
            QJsonArray jsonArray = jsonDoc.array();
            for (int i = 0; i < jsonArray.size(); i++) {
                QJsonObject obj = jsonArray.at(i).toObject();
                qDebug() << "name:" << obj.value("zh").toString() << "type:" << obj.value("type").toInt();
            }

            //    qDebug() << configJson;
            return true;
        }

        QString TreeConfigWidget::getLocalLanguage() {
            QLocale locale;
            QString language = QLocale::languageToString(locale.language());
            return language;
        }
    }
}