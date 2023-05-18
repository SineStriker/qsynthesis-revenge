#include "TreeConfigWidget.h"
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QListView>
#include <QLocale>
#include <QMessageBox>
#include <QSpinBox>
#include <QVBoxLayout>

namespace TemplatePlg {
    namespace Internal {
        TreeConfigWidget::TreeConfigWidget(QString configPath, bool configGen, QWidget *parent)
            : configPath(configPath), configGen(configGen), QWidget(parent) {
            mainLayout = new QVBoxLayout(this);

            auto treeLayout = new QHBoxLayout();
            m_treeWidget = new QTreeWidget();
            m_treeWidget->setColumnCount(3);
            m_treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
            treeLayout->addWidget(m_treeWidget);

            if (getLocalLanguage() == "Chinese") {
                m_treeWidget->setHeaderLabels({"属性", "英文属性", "值"});
            } else {
                m_treeWidget->setHeaderLabels({"zh Key", "Key", "Value"});
            }

            if (configGen) {
                auto buttonLayout = new QVBoxLayout();

                // zh name
                auto nameLayout = new QHBoxLayout();
                auto m_name_label = new QLabel(tr("Key"));
                m_name = new QLineEdit();
                nameLayout->addWidget(m_name_label);
                nameLayout->addWidget(m_name);
                buttonLayout->addLayout(nameLayout);

                // en name
                auto enNameLayout = new QHBoxLayout();
                auto m_enName_label = new QLabel(tr("En Key"));
                m_enName = new QLineEdit();
                enNameLayout->addWidget(m_enName_label);
                enNameLayout->addWidget(m_enName);
                buttonLayout->addLayout(enNameLayout);

                // format
                auto formatLayout = new QHBoxLayout();
                auto m_format_label = new QLabel(tr("Format Expected"));
                m_format = new QLabel();
                formatLayout->addWidget(m_format_label);
                formatLayout->addWidget(m_format);
                buttonLayout->addLayout(formatLayout);

                // value
                auto valueLayout = new QHBoxLayout();
                auto m_value_label = new QLabel(tr("Value"));
                m_value = new QLineEdit();
                valueLayout->addWidget(m_value_label);
                valueLayout->addWidget(m_value);
                buttonLayout->addLayout(valueLayout);

                // childType
                auto childLayout = new QHBoxLayout();
                auto m_child_label = new QLabel(tr("Insert Hierarchy"));
                m_childType = new QComboBox();
                m_childType->addItem(tr("Top"));
                m_childType->addItem(tr("Child"));
                childLayout->addWidget(m_child_label);
                childLayout->addWidget(m_childType);
                buttonLayout->addLayout(childLayout);

                // controlType
                auto controlLayout = new QHBoxLayout();
                auto m_control_label = new QLabel(tr("Control Type"));
                m_type = new QComboBox();
                m_type->addItem(tr("Group"));
                m_type->addItem(tr("QCheckBox"));
                m_type->addItem(tr("QComboBox"));
                m_type->addItem(tr("QLineEdit"));
                m_type->addItem(tr("QSpinBox"));
                controlLayout->addWidget(m_control_label);
                controlLayout->addWidget(m_type);
                buttonLayout->addLayout(controlLayout);

                auto moveLayout = new QHBoxLayout();
                m_up = new QPushButton("↑");
                m_down = new QPushButton("↓");
                moveLayout->addWidget(m_up);
                moveLayout->addWidget(m_down);

                m_addButton = new QPushButton(tr("add"));
                m_removeButton = new QPushButton(tr("remove"));
                m_saveButton = new QPushButton(tr("save"));
                m_loadButton = new QPushButton(tr("load"));

                buttonLayout->addLayout(moveLayout);
                buttonLayout->addWidget(m_addButton);
                buttonLayout->addWidget(m_removeButton);
                buttonLayout->addWidget(m_loadButton);
                buttonLayout->addWidget(m_saveButton);

                treeLayout->addLayout(buttonLayout);

                connect(m_type, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                        &TreeConfigWidget::on_format_Changed);
                connect(m_up, &QPushButton::clicked, this, &TreeConfigWidget::on_btnUp_clicked);
                connect(m_down, &QPushButton::clicked, this, &TreeConfigWidget::on_btnDown_clicked);
                connect(m_addButton, &QPushButton::clicked, this, &TreeConfigWidget::addTableRow);
                connect(m_removeButton, &QPushButton::clicked, this, &TreeConfigWidget::removeTableRow);
                connect(m_loadButton, &QPushButton::clicked, this, &TreeConfigWidget::createConfig);
                mainLayout->addLayout(treeLayout);
            } else {
                if (getLocalLanguage() == "Chinese") {
                    m_treeWidget->hideColumn(1);
                } else {
                    m_treeWidget->hideColumn(0);
                }
                m_saveButton = new QPushButton(tr("save"));
                mainLayout->addLayout(treeLayout);
                mainLayout->addWidget(m_saveButton);
                loadConfig(readJsonFile(configPath));
            }
            connect(m_saveButton, &QPushButton::clicked, this, &TreeConfigWidget::saveConfig);
            m_treeWidget->expandAll();
        }

        TreeConfigWidget::~TreeConfigWidget() {
        }

        void TreeConfigWidget::on_format_Changed(int index) {
            qDebug() << index;
            switch (index) {
                case 0: {
                    m_format->setText(tr("null"));
                    break;
                }
                case 1: {
                    m_format->setText(tr("bool"));
                    break;
                }
                case 2: {
                    m_format->setText("中文;中文;En;En");
                    break;
                }
                case 3: {
                    m_format->setText(tr("lineEditValue"));
                    break;
                }
                case 4: {
                    m_format->setText(tr("min;max;step(int)"));
                    break;
                }
                default:
                    break;
            }
        }

        void TreeConfigWidget::addTableRow() {
            auto itemName = m_name->text();
            auto itemEnName = m_enName->text();
            auto value = m_value->text();

            auto *Label = new QLabel();
            QTreeWidgetItem *child = new QTreeWidgetItem(QStringList() << itemName << itemEnName);
            child->setFlags(child->flags() | Qt::ItemIsEditable);
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
                    break;
                }
                case 1: {
                    auto *frame = new QCheckBox();
                    frame->setChecked(QVariant(value).toBool());
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
                        mb.setText(tr(
                            "Please input\r\n\"Chinese; Chinese; English; English\" in the following format to ensure "
                            "that the items are even numbers, and click addButton again."));
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
                case 4: {
                    auto *frame = new QSpinBox();
                    auto values = value.split(";");
                    frame->setMinimum(values[0].toInt());
                    frame->setMaximum(values[1].toInt());
                    frame->setSingleStep(values[2].toInt());
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

        QJsonArray TreeConfigWidget::createJsonFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item) {
            QJsonArray json;
            // root item
            if (!item) {
                item = treeWidget->invisibleRootItem();
            }
            // child item
            for (int i = 0; i < item->childCount(); i++) {
                QTreeWidgetItem *childItem = item->child(i);
                QString type;
                auto itemWidget = treeWidget->itemWidget(childItem, 2);

                QJsonObject childJson;
                childJson["zh"] = childItem->text(0);
                childJson["en"] = childItem->text(1);

                if (qobject_cast<QCheckBox *>(itemWidget)) {
                    type = "QCheckBox";
                    childJson["value"] = qobject_cast<QCheckBox *>(itemWidget)->isChecked();
                } else if (qobject_cast<QComboBox *>(itemWidget)) {
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
                } else if (qobject_cast<QLineEdit *>(itemWidget)) {
                    type = "QLineEdit";
                    childJson["value"] = qobject_cast<QLineEdit *>(itemWidget)->text();
                } else if (qobject_cast<QSpinBox *>(itemWidget)) {
                    auto sb = qobject_cast<QSpinBox *>(itemWidget);
                    type = "QSpinBox";
                    childJson["index"] = sb->value();
                    childJson["value"] = QJsonArray() << sb->minimum() << sb->maximum() << sb->singleStep();
                } else {
                    type = "Group";
                }

                childJson["type"] = type;
                if (type == "Group" && childItem->childCount()) {
                    childJson["value"] = createJsonFromTree(treeWidget, childItem);
                    json.insert(i, childJson);
                } else {
                    json.insert(i, childJson);
                }
            }

            return json;
        }

        QJsonArray TreeConfigWidget::readJsonFile(QString filePath) {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Failed to open file: " << filePath;
                return QJsonArray();
            }

            QByteArray jsonData = file.readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (!jsonDoc.isArray()) {
                qDebug() << "Invalid JSON format: " << filePath;
                return QJsonArray();
            }

            return jsonDoc.array();
        }

        void TreeConfigWidget::loadConfig(const QJsonArray &config, int insertIndex, QTreeWidgetItem *parent) {
            for (auto it = config.begin(); it != config.end(); ++it) {
                QTreeWidgetItem *item = new QTreeWidgetItem();
                // must insert item before setting it
                if (parent) {
                    if (insertIndex >= 0) {
                        parent->insertChild(insertIndex, item);
                    } else {
                        parent->addChild(item);
                    }
                } else {
                    if (insertIndex >= 0) {
                        m_treeWidget->insertTopLevelItem(insertIndex, item);
                    } else {
                        m_treeWidget->addTopLevelItem(item);
                    }
                }
                if (configGen) {
                    item->setFlags(item->flags() | Qt::ItemIsEditable);
                }
                // set zh and en columns
                item->setText(0, it->toObject().value("zh").toString());
                item->setText(1, it->toObject().value("en").toString());

                QString type = it->toObject().value("type").toString();
                if (type == "Group") {
                    QLabel *label = new QLabel(m_treeWidget);
                    m_treeWidget->setItemWidget(item, 2, label);
                } else if (type == "QComboBox") {
                    QComboBox *comboBox = new QComboBox(m_treeWidget);
                    int index = it->toObject().value("index").toInt();

                    QStringList zhValues = it->toObject().value("zh_value").toVariant().toStringList();
                    QStringList enValues = it->toObject().value("en_value").toVariant().toStringList();
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
                    checkBox->setChecked(it->toObject().value("value").toBool());
                    m_treeWidget->setItemWidget(item, 2, checkBox);
                } else if (type == "QLineEdit") {
                    // Insert QLineEdit for QLineEdit type
                    QLineEdit *lineEdit = new QLineEdit(m_treeWidget);
                    lineEdit->setText(it->toObject().value("value").toString());
                    m_treeWidget->setItemWidget(item, 2, lineEdit);
                } else if (type == "QSpinBox") {
                    // Insert QLineEdit for QLineEdit type
                    QSpinBox *spinBox = new QSpinBox(m_treeWidget);
                    int index = it->toObject().value("index").toInt();
                    QStringList values = it->toObject().value("value").toVariant().toStringList();
                    spinBox->setMinimum(values[0].toInt());
                    spinBox->setMaximum(values[1].toInt());
                    spinBox->setSingleStep(values[2].toInt());
                    spinBox->setValue(index);
                    m_treeWidget->setItemWidget(item, 2, spinBox);
                }


                if (type == "Group") {
                    // recursively load child items
                    loadConfig(it->toObject().value("value").toArray(), -1, item);
                }
            }
        }

        bool TreeConfigWidget::saveConfig() {
            QJsonArray configJson = createJsonFromTree(m_treeWidget);
            auto filePath = configGen ? developConfigPath : configPath;
            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                // Failed to open file
                return false;
            }

            QJsonDocument jsonDoc(configJson);
            file.write(jsonDoc.toJson());
            return true;
        }

        void TreeConfigWidget::createConfig() {
            developConfigPath =
                QFileDialog::getOpenFileName(nullptr, "Open File", "", "Json Files (*.json);;All Files (*)");
            loadConfig(readJsonFile(developConfigPath));
            m_treeWidget->expandAll();
        }

        QString TreeConfigWidget::getLocalLanguage() {
            QLocale locale;
            QString language = QLocale::languageToString(locale.language());
            return language;
        }

        // move item up
        void TreeConfigWidget::on_btnUp_clicked() {
            QTreeWidgetItem *item = m_treeWidget->currentItem();
            if (item) {
                QTreeWidgetItem *parent = item->parent();
                int index = parent ? parent->indexOfChild(item) : m_treeWidget->indexOfTopLevelItem(item);
                if (index > 0) {
                    QJsonArray data;
                    data.insert(0, createJsonFromTree(m_treeWidget, parent).at(index).toObject());
                    if (parent == nullptr) {
                        m_treeWidget->takeTopLevelItem(index);
                    } else {
                        parent->takeChild(index);
                    }
                    loadConfig(data, index - 1, parent);
                    auto cItem = parent ? parent->child(index - 1) : m_treeWidget->topLevelItem(index - 1);
                    m_treeWidget->setCurrentItem(cItem);
                    m_treeWidget->expandItem(cItem);
                }
            }
        }

        // move item down
        void TreeConfigWidget::on_btnDown_clicked() {
            QTreeWidgetItem *item = m_treeWidget->currentItem();
            if (item) {
                QTreeWidgetItem *parent = item->parent();
                int index = parent ? parent->indexOfChild(item) : m_treeWidget->indexOfTopLevelItem(item);
                QJsonArray data;
                data.insert(0, createJsonFromTree(m_treeWidget, parent).at(index).toObject());
                auto maxIndex = parent ? parent->childCount() - 1 : m_treeWidget->topLevelItemCount() - 1;
                if (index < maxIndex) {
                    if (parent == nullptr) {
                        m_treeWidget->takeTopLevelItem(index);
                        loadConfig(data, index + 1, parent);
                    } else {
                        parent->takeChild(index);
                        loadConfig(data, index + 1, parent);
                    }
                    auto cItem = parent ? parent->child(index + 1) : m_treeWidget->topLevelItem(index + 1);
                    m_treeWidget->setCurrentItem(cItem);
                    m_treeWidget->expandItem(cItem);
                }
            }
        }

    }
}