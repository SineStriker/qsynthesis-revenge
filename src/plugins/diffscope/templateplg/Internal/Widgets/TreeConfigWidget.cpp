#include "TreeConfigWidget.h"
#include "TreeDevWidget.h"
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

        static TreeConfigWidget *m_instance = nullptr;

        TreeConfigWidget::TreeConfigWidget(QString configDir, bool configGen, QWidget *parent)
            : configPath(configDir + "config.json"), uiPath(configDir + "config.treeui"), configGen(configGen),
              m_language(TreeJsonUtil::getLocalLanguage()), QWidget(parent) {
            m_instance = this;
            m_widget = createWidget();
            m_widget->setParent(parent);
        }

        TreeConfigWidget::~TreeConfigWidget() {
            m_instance = nullptr;
        }

        QWidget *TreeConfigWidget::configWidget() {
            return createWidget();
        }

        QWidget *TreeConfigWidget::createWidget() {
            auto mainWidget = new QWidget;
            mainLayout = new QVBoxLayout(mainWidget);
            auto treeLayout = treeWidgetBox();
            auto bottomLayout = bottomButtonBox();

            if (configGen) {
                auto devWidget = new TreeDevWidget(m_treeWidget);
                m_defaultButton->setText(tr("creat treeui"));
                m_saveButton->setText(tr("save ui&config info"));
                treeLayout->addWidget(devWidget);
            } else {
                TreeJsonUtil::TreeFromFile(uiPath, configGen, m_treeWidget);
                if (QFile::exists(configPath)) {
                    loadConfig(TreeJsonUtil::JsonObjectFromFile(configPath), m_treeWidget);
                    configModel = TreeJsonUtil::JsonObjectFromTree(m_treeWidget);
                } else {
                    auto m_box = TreeJsonUtil::messageBox(
                        tr("File not exist!"),
                        tr("The setting information file (config. json) does not exist, use default settings!"));
                    m_box->exec();
                    TreeJsonUtil::TreeToFile(configPath, m_treeWidget);
                }
                if (m_language == "Chinese") {
                    m_treeWidget->hideColumn(1);
                } else {
                    m_treeWidget->hideColumn(0);
                }
            }

            mainLayout->addLayout(treeLayout);
            mainLayout->addLayout(bottomLayout);
            m_treeWidget->expandAll();
            return mainWidget;
        }

        QHBoxLayout *TreeConfigWidget::treeWidgetBox() {
            auto treeLayout = new QHBoxLayout();
            m_treeWidget = new QTreeWidget();
            m_treeWidget->setColumnCount(3);
            m_treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
            treeLayout->addWidget(m_treeWidget);

            if (m_language == "Chinese") {
                m_treeWidget->setHeaderLabels({tr("Key"), tr("En Key"), tr("Value")});
            } else {
                m_treeWidget->setHeaderLabels({"zh Key", "Key", "Value"});
            }
            return treeLayout;
        }

        QHBoxLayout *TreeConfigWidget::bottomButtonBox() {
            auto bottomLayout = new QHBoxLayout();
            m_defaultButton = new QPushButton(tr("default config"));
            m_loadButton = new QPushButton(tr("load config"));
            m_saveButton = new QPushButton(tr("save config"));
            bottomLayout->addWidget(m_defaultButton);
            bottomLayout->addWidget(m_loadButton);
            bottomLayout->addWidget(m_saveButton);

            connect(m_defaultButton, &QPushButton::clicked, this, &TreeConfigWidget::on_default_clicked);
            connect(m_loadButton, &QPushButton::clicked, this, &TreeConfigWidget::on_loadConfig_clicked);
            connect(m_saveButton, &QPushButton::clicked, this, &TreeConfigWidget::on_save_clicked);
            return bottomLayout;
        }

        bool TreeConfigWidget::on_save_clicked() {
            QJsonArray configJson = TreeJsonUtil::JsonArrayFromTree(m_treeWidget);
            bool res = TreeJsonUtil::JsonArrayToFile(configPath, configJson);
            if (res) {
                auto m_box = TreeJsonUtil::messageBox(tr("Successfully saved"), tr("Successfully saved!"));
                m_box->exec();
            }
            if (configGen) {
                TreeJsonUtil::JsonArrayToFile(developUiPath, configJson);
                TreeJsonUtil::JsonArrayToFile(developUiPath.replace(".treeui", ".json"), configJson);
                TreeJsonUtil::JsonArrayToFile(configPath.replace(".json", ".treeui"), configJson);
            }
            configModel = TreeJsonUtil::JsonObjectFromFile(configPath);
            return true;
        }

        QVariant TreeConfigWidget::readConfig(const QString path, QString type) {
            QStringList pathList = path.split("/");
            QJsonValue jsonValue = configModel;
            for (const auto &pathItem : pathList) {
                jsonValue = jsonValue.toObject()[pathItem];
            }
            if (jsonValue["type"] == "QComboBox") {
                return type == "index"
                           ? QVariant(jsonValue["index"])
                           : QVariant(jsonValue["en_value"].toVariant().toStringList()[jsonValue["index"].toInt()]);
            } else {
                return QVariant(jsonValue["value"]);
            }
        }

        TreeConfigWidget *TreeConfigWidget::Instance() {
            return m_instance;
        }

        void TreeConfigWidget::loadConfig(const QJsonObject configObj, QTreeWidget *treeWidget, QTreeWidgetItem *item) {
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
                        if (index != -1) {
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

        void TreeConfigWidget::on_default_clicked() {
            qDeleteAll(m_treeWidget->invisibleRootItem()->takeChildren());
            if (configGen) {
                developUiPath =
                    QFileDialog::getOpenFileName(nullptr, "Open File", "", "Tree Ui Files (*.treeui);;All Files (*)");
                TreeJsonUtil::TreeFromFile(developUiPath, configGen, m_treeWidget);
            } else {
                TreeJsonUtil::TreeFromFile(uiPath, configGen, m_treeWidget);
            }
            m_treeWidget->expandAll();
        }

        void TreeConfigWidget::on_loadConfig_clicked() {
            configPath = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Json Files (*.json);;All Files (*)");
            loadConfig(TreeJsonUtil::JsonObjectFromFile(configPath), m_treeWidget);
            m_treeWidget->expandAll();
        }



    }
}