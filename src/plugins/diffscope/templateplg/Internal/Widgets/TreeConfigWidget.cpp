#include "TreeConfigWidget.h"
#include "TreeDevWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QLineEdit>
#include <QListView>
#include <QLocale>
#include <QMessageBox>
#include <QSpinBox>
#include <QVBoxLayout>

namespace TemplatePlg::Internal {
        static QHash<QString, TreeConfigWidget *> m_instances;
        TreeConfigWidget::TreeConfigWidget(const QString &pluginId, const QString &configDir, bool configGen,
                                           QWidget *parent)
            : pluginId(pluginId), configPath(configDir + "config.json"), uiPath(configDir + "config.treeui"),
              configGen(configGen), m_language(TreeJsonUtil::getLocalLanguage()), QWidget(parent) {
            m_instances.insert(pluginId, this);
            m_widget = createWidget();
            m_treeWidget->setHorizontalScrollMode(QTreeView::ScrollPerPixel);
            m_widget->setParent(parent);
        }

        TreeConfigWidget::~TreeConfigWidget() {
            m_instances.clear();
        }

        QWidget *TreeConfigWidget::configWidget() {
            return createWidget();
        }

        QHBoxLayout *TreeConfigWidget::treeWidgetBox() {
            auto treeLayout = new QHBoxLayout();
            m_treeWidget = new QTreeWidget();
            m_treeWidget->setColumnCount(3);
            m_treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
            treeLayout->addWidget(m_treeWidget);

            if (m_language == "Chinese") {
                m_treeWidget->setHeaderLabels({tr("Key"), tr("En Key"), tr("Value"), tr("Remark"), tr("En Remark")});
            } else {
                m_treeWidget->setHeaderLabels({"zh Key", "Key", "Value", "Remark", "En Remark"});
            }
            return treeLayout;
        }

        QWidget *TreeConfigWidget::createWidget() {
            auto mainWidget = new QWidget;
            mainLayout = new QVBoxLayout(mainWidget);
            auto treeLayout = treeWidgetBox();
            auto bottomLayout = bottomButtonBox();

            if (configGen) {
                auto devWidget = new TreeDevWidget(m_treeWidget);
                m_defaultButton->setText(tr("create treeui"));
                m_saveButton->setText(tr("save ui&config info"));
                devWidget->setMaximumWidth(300);
                treeLayout->addWidget(devWidget);
            } else {
                if (QFile::exists(uiPath)) {
                    TreeJsonUtil::TreeFromFile(uiPath, configGen, m_treeWidget);
                } else {
                    QMessageBox::information(this, tr("Warning"),
                                    pluginId + tr(": The configuration interface (config.treeui) is missing. "
                                                  "Please download and install this plugin again."));
                }
                if (QFile::exists(configPath)) {
                    loadConfig(TreeJsonUtil::jsonObjectFromFile(configPath), m_treeWidget);
                    configModel = TreeJsonUtil::jsonObjectFromTree(m_treeWidget);
                } else {
                    QMessageBox::information(
                        this, tr("File not exist!"),
                        pluginId +
                            tr(": The setting information file (config.json) does not exist, use default settings!"));

                    TreeJsonUtil::TreeToFile(configPath, m_treeWidget);
                }
                if (m_language == "Chinese") {
                    m_treeWidget->hideColumn(1);
                    m_treeWidget->hideColumn(4);
                } else {
                    m_treeWidget->hideColumn(0);
                    m_treeWidget->hideColumn(3);
                }
            }

            mainLayout->addLayout(treeLayout);
            mainLayout->addLayout(bottomLayout);
            m_treeWidget->expandAll();
            return mainWidget;
        }

        QHBoxLayout *TreeConfigWidget::bottomButtonBox() {
            auto bottomLayout = new QHBoxLayout();
            m_defaultButton = new QPushButton(tr("default config"));
            m_loadButton = new QPushButton(tr("load config"));
            m_saveButton = new QPushButton(tr("save config"));
            bottomLayout->addWidget(m_defaultButton);
            bottomLayout->addWidget(m_loadButton);
            bottomLayout->addWidget(m_saveButton);

            connect(m_defaultButton, &QPushButton::clicked, this, &TreeConfigWidget::_q_defaultClicked);
            connect(m_loadButton, &QPushButton::clicked, this, &TreeConfigWidget::_q_loadConfigClicked);
            connect(m_saveButton, &QPushButton::clicked, this, &TreeConfigWidget::_q_saveClicked);
            return bottomLayout;
        }

        QVariant TreeConfigWidget::readConfig(const QString &path, const QString& type) {
            QStringList pathList = path.contains("/") ? path.split("/") : QStringList() << path;
            QJsonValue jsonValue = configModel;
            for (const auto &pathItem : pathList) {
                auto jsonObj = jsonValue.toObject();
                if (jsonObj.contains(pathItem)) {
                    jsonValue = jsonValue[pathItem];
                } else {
                    return {};
                }
            }
            if (jsonValue["type"] == "QComboBox") {
                return type == "index"
                           ? QVariant(jsonValue["index"])
                           : QVariant(jsonValue["en_value"].toVariant().toStringList()[jsonValue["index"].toInt()]);
            } else {
                return jsonValue["value"];
            }
        }

        TreeConfigWidget *TreeConfigWidget::instance(const QString &pluginId) {
            if (m_instances.contains(pluginId)) {
                return m_instances.value(pluginId);
            } else {
                return nullptr;
            }
        }

        void TreeConfigWidget::loadConfig(const QJsonObject &configObj, QTreeWidget *treeWidget,
                                          QTreeWidgetItem *item) {
            // root item
            if (!item) {
                item = treeWidget->invisibleRootItem();
            }
            // child item
            for (int i = 0; i < item->childCount(); i++) {
                QTreeWidgetItem *childItem = item->child(i);
                QString key = childItem->text(1);
                if (configObj.contains(key)) {
                    QJsonObject childJson = configObj.value(key).toObject();
                    auto itemWidget = treeWidget->itemWidget(childItem, 2);
                    QString childType = childJson["type"].toString();

                    if (qobject_cast<QCheckBox *>(itemWidget) && childType == "QCheckBox") {
                        qobject_cast<QCheckBox *>(itemWidget)->setChecked(childJson["value"].toBool());
                    } else if (qobject_cast<QComboBox *>(itemWidget) && childType == "QComboBox") {
                        auto cb = qobject_cast<QComboBox *>(itemWidget);
                        auto cbList = childJson["en_value"].toVariant().toStringList();
                        auto index = cb->findText(cbList[childJson["index"].toInt()]);
                        if (index != -1 && m_language == "Chinese") {
                            cb->setCurrentIndex(index - cbList.size());
                        } else if (index != 1) {
                            cb->setCurrentIndex(index);
                        }
                    } else if (qobject_cast<QLineEdit *>(itemWidget) && childType == "QLineEdit") {
                        qobject_cast<QLineEdit *>(itemWidget)->setText(childJson["value"].toString());
                    } else if (qobject_cast<QSpinBox *>(itemWidget) && childType == "QSpinBox") {
                        auto sb = qobject_cast<QSpinBox *>(itemWidget);
                        sb->setValue(childJson["value"].toInt());
                    } else if (qobject_cast<QWidget *>(itemWidget) && childType == "QFileDialog") {
                        auto fd = qobject_cast<QWidget *>(itemWidget)->findChild<QLineEdit *>();
                        fd->setText(childJson["value"].toString());
                    } else {
                        if (childItem->childCount()) {
                            loadConfig(childJson, treeWidget, childItem);
                        }
                    }
                }
            }
        }

        void TreeConfigWidget::_q_defaultClicked() {
            qDeleteAll(m_treeWidget->invisibleRootItem()->takeChildren());
            if (configGen) {
                developUiPath =
                    QFileDialog::getOpenFileName(nullptr, "Open File", "", "Tree Ui Files (*.treeui);;All Files (*)");
                TreeJsonUtil::TreeFromFile(developUiPath, configGen, m_treeWidget);
                m_treeWidget->expandAll();
            } else {
                TreeJsonUtil::TreeFromFile(uiPath, configGen, m_treeWidget);
                m_treeWidget->expandAll();
                QMessageBox::information(this, tr("Info"),
                                tr("The default settings have been restored. Please manually "
                                   "click the save button to confirm the modification."));
            }
        }

        void TreeConfigWidget::_q_loadConfigClicked() {
            configPath = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Json Files (*.json);;All Files (*)");
            loadConfig(TreeJsonUtil::jsonObjectFromFile(configPath), m_treeWidget);
            m_treeWidget->expandAll();
        }

        bool TreeConfigWidget::_q_saveClicked() {
            QJsonArray configJson = TreeJsonUtil::jsonArrayFromTree(m_treeWidget);
            bool res = TreeJsonUtil::jsonArrayToFile(configPath, configJson);
            if (res) {
                QMessageBox::information(this, tr("Successfully saved"), tr("Successfully saved!"));
            }
            if (configGen) {
                QFileInfo devUi(developUiPath);
                QFileInfo configInfo(configPath);
                TreeJsonUtil::jsonArrayToFile(developUiPath, configJson);
                TreeJsonUtil::jsonArrayToFile(devUi.path() + "/" + devUi.completeBaseName() + ".json", configJson);
                TreeJsonUtil::jsonArrayToFile(configInfo.path() + "/" + devUi.completeBaseName() + ".treeui", configJson);
            }
            configModel = TreeJsonUtil::jsonObjectFromFile(configPath);
            return true;
        }

    }