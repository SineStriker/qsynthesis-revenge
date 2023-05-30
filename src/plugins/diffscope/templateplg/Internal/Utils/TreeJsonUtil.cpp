#include "TreeJsonUtil.h"
#include "QPushButton"
#include "QTreeWidget"
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QLocale>
#include <QSpinBox>

namespace TemplatePlg::Internal {
        QJsonArray TreeJsonUtil::jsonArrayFromFile(const QString &filePath) {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "Failed to open file: " << filePath;
                return {};
            }
            QByteArray jsonData = file.readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (!jsonDoc.isArray()) {
                qDebug() << "Invalid JSON format: " << filePath;
                return {};
            }
            return jsonDoc.array();
        }

        QJsonObject TreeJsonUtil::jsonObjectFromFile(const QString &filePath) {
            return jsonArrayToJsonObject(jsonArrayFromFile(filePath));
        }

        bool TreeJsonUtil::jsonArrayToFile(const QString &filePath, const QJsonArray &configJson) {
            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                // Failed to open file
                return false;
            }
            QJsonDocument jsonDoc(configJson);
            file.write(jsonDoc.toJson());
            return true;
        }

        bool TreeJsonUtil::TreeToFile(const QString &filePath, QTreeWidget *treeWidget) {
            return jsonArrayToFile(filePath, jsonArrayFromTree(treeWidget));
        }

        QJsonObject TreeJsonUtil::jsonArrayToJsonObject(const QJsonArray &jsonArray) {
            QJsonObject jsonObject;

            for (const auto &jsonValue : jsonArray) {
                const auto &subJsonObject = jsonValue.toObject();
                const QString &key = subJsonObject["en"].toString();

                if (subJsonObject["type"].toString() == "Group") {
                    jsonObject[key] = jsonArrayToJsonObject(subJsonObject["value"].toArray());
                } else {
                    jsonObject[key] = subJsonObject;
                }
            }
            return jsonObject;
        }

        QJsonArray TreeJsonUtil::jsonArrayFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item) {
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
                childJson["remark"] = childItem->text(3) + ";" + childItem->text(4);

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
                    childJson["value"] = sb->value();
                    childJson["content"] = QJsonArray() << sb->minimum() << sb->maximum() << sb->singleStep();
                } else if (qobject_cast<QWidget *>(itemWidget)->findChild<QLineEdit *>()) {
                    auto lineEdit = qobject_cast<QWidget *>(itemWidget)->findChild<QLineEdit *>();
                    type = "QFileDialog";
                    childJson["value"] = lineEdit->text();
                } else {
                    type = "Group";
                }

                childJson["type"] = type;
                if (type == "Group" && childItem->childCount()) {
                    childJson["value"] = jsonArrayFromTree(treeWidget, childItem);
                } else if (type == "Group" && childItem->childCount() == 0) {
                    childJson["value"] = QJsonArray();
                }
                json.insert(i, childJson);
            }

            return json;
        }

        QJsonObject TreeJsonUtil::jsonObjectFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item) {
            return jsonArrayToJsonObject(jsonArrayFromTree(treeWidget, item));
        }

        QString TreeJsonUtil::getLocalLanguage() {
            QLocale locale;
            QString language = QLocale::languageToString(locale.language());
            return language;
        }

        void TreeJsonUtil::TreeFromJsonArray(const QJsonArray &config, int insertIndex, bool configGen,
                                             QTreeWidget *treeWidget, QTreeWidgetItem *parent) {
            auto language = getLocalLanguage();
            for (auto it = config.begin(); it != config.end(); ++it) {
                auto *item = new QTreeWidgetItem();
                // must insert item before setting it
                if (parent) {
                    if (insertIndex >= 0) {
                        parent->insertChild(insertIndex, item);
                    } else {
                        parent->addChild(item);
                    }
                } else {
                    if (insertIndex >= 0) {
                        treeWidget->insertTopLevelItem(insertIndex, item);
                    } else {
                        treeWidget->addTopLevelItem(item);
                    }
                }
                if (configGen) {
                    item->setFlags(item->flags() | Qt::ItemIsEditable);
                }
                // set zh and en columns
                item->setText(0, it->toObject().value("zh").toString());
                item->setText(1, it->toObject().value("en").toString());
                auto remarks = it->toObject().value("remark").toString().split(";");
                item->setText(3, remarks[0]);
                item->setText(4, remarks[1]);

                QString type = it->toObject().value("type").toString();
                if (type == "Group") {
                    auto *label = new QLabel(treeWidget);
                    treeWidget->setItemWidget(item, 2, label);
                } else if (type == "QComboBox") {
                    auto *comboBox = new QComboBox(treeWidget);
                    int index = it->toObject().value("index").toInt();

                    QStringList zhValues = it->toObject().value("zh_value").toVariant().toStringList();
                    QStringList enValues = it->toObject().value("en_value").toVariant().toStringList();
                    auto *view = qobject_cast<QListView *>(comboBox->view());
                    comboBox->addItems(zhValues);
                    comboBox->addItems(enValues);
                    if (!configGen && language == "Chinese") {
                        for (int c_i = 0; c_i < zhValues.size(); c_i++) {
                            view->setRowHidden(c_i + zhValues.size(), true);
                        }
                        index = index < zhValues.size() ? index : index - zhValues.size();
                    } else if (!configGen && language != "Chinese") {
                        for (int c_i = 0; c_i < zhValues.size(); c_i++) {
                            view->setRowHidden(c_i, true);
                        }
                        index = index > zhValues.size() ? index : index + zhValues.size();
                    }
                    comboBox->setCurrentIndex(index);
                    treeWidget->setItemWidget(item, 2, comboBox);
                } else if (type == "QCheckBox") {
                    // Insert QCheckBox for QCheckBox type
                    auto *checkBox = new QCheckBox(treeWidget);
                    checkBox->setChecked(it->toObject().value("value").toBool());
                    treeWidget->setItemWidget(item, 2, checkBox);
                } else if (type == "QLineEdit") {
                    // Insert QLineEdit for QLineEdit type
                    auto *lineEdit = new QLineEdit(treeWidget);
                    lineEdit->setText(it->toObject().value("value").toString());
                    treeWidget->setItemWidget(item, 2, lineEdit);
                } else if (type == "QSpinBox") {
                    // Insert QLineEdit for QLineEdit type
                    auto *spinBox = new QSpinBox(treeWidget);
                    int index = it->toObject().value("index").toInt();
                    QStringList values = it->toObject().value("content").toVariant().toStringList();
                    spinBox->setRange(values[0].toInt(), values[1].toInt());
                    spinBox->setSingleStep(values[2].toInt());
                    spinBox->setValue(index);
                    treeWidget->setItemWidget(item, 2, spinBox);
                } else if (type == "QFileDialog") {
                    auto widget = qFileWidget(it->toObject().value("value").toString());
                    treeWidget->setItemWidget(item, 2, widget);
                }

                if (type == "Group") {
                    // recursively load child items
                    TreeFromJsonArray(it->toObject().value("value").toArray(), -1, configGen, treeWidget, item);
                }
            }
        }

        void TreeJsonUtil::TreeFromFile(const QString &filePath, bool configGen, QTreeWidget *treeWidget) {
            TreeFromJsonArray(jsonArrayFromFile(filePath), -1, configGen, treeWidget);
        }

        QWidget *TreeJsonUtil::qFileWidget(const QString &text) {
            auto *widget = new QWidget();
            auto *layout = new QHBoxLayout();
            auto *button = new QPushButton("...");
            auto *lineEdit = new QLineEdit();
            if (text != "") {
                lineEdit->setText(text);
            }
            layout->addWidget(lineEdit);
            layout->addWidget(button);
            widget->setLayout(layout);
            QObject::connect(button, &QPushButton::clicked, [=]() {
                QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "All Files (*)");
                lineEdit->setText(fileName);
            });
            return widget;
        }
    } // TemplatePlg