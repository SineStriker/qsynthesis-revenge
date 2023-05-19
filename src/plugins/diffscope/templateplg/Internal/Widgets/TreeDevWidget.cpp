#include "TreeDevWidget.h"
#include "../Utils/TreeJsonUtil.h"

#include <QCheckBox>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QSpinBox>
#include <QTreeWidget>

namespace TemplatePlg {
    namespace Internal {
        TreeDevWidget::TreeDevWidget(QTreeWidget *m_treeWidget, QWidget *parent)
            : m_treeWidget(m_treeWidget), QWidget(parent) {
            auto buttonLayout = new QVBoxLayout();
            buttonLayout->addStretch();

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
            m_format = new QLabel(tr("null"));
            formatLayout->addWidget(m_format_label);
            formatLayout->addStretch();
            formatLayout->addWidget(m_format);
            formatLayout->addStretch();
            buttonLayout->addLayout(formatLayout);

            // value
            auto valueLayout = new QHBoxLayout();
            m_value = new QLineEdit();
            valueLayout->addWidget(m_value);
            buttonLayout->addLayout(valueLayout);

            // tip
            auto tipLayout = new QVBoxLayout();
            auto m_tip_label = new QLabel(tr("Remark(format: Zh;En or null)"));
            m_remark = new QLineEdit();
            tipLayout->addWidget(m_tip_label);
            tipLayout->addWidget(m_remark);
            buttonLayout->addLayout(tipLayout);
            buttonLayout->addStretch();

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
            m_type->addItem(tr("QFileDialog"));
            controlLayout->addWidget(m_control_label);
            controlLayout->addWidget(m_type);
            buttonLayout->addLayout(controlLayout);

            // moveButton
            auto moveLayout = new QHBoxLayout();
            m_up = new QPushButton("↑");
            m_down = new QPushButton("↓");
            moveLayout->addWidget(m_up);
            moveLayout->addWidget(m_down);

            m_addButton = new QPushButton(tr("add"));
            m_removeButton = new QPushButton(tr("remove"));

            buttonLayout->addLayout(moveLayout);
            buttonLayout->addWidget(m_addButton);
            buttonLayout->addWidget(m_removeButton);
            buttonLayout->addStretch();

            // slots
            connect(m_type, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                    &TreeDevWidget::on_format_Changed);
            connect(m_up, &QPushButton::clicked, this, &TreeDevWidget::on_btnUp_clicked);
            connect(m_down, &QPushButton::clicked, this, &TreeDevWidget::on_btnDown_clicked);
            connect(m_addButton, &QPushButton::clicked, this, &TreeDevWidget::on_add_clicked);
            connect(m_removeButton, &QPushButton::clicked, this, &TreeDevWidget::on_remove_clicked);

            setLayout(buttonLayout);
        }

        TreeDevWidget::~TreeDevWidget() {
        }

        void TreeDevWidget::on_format_Changed(int index) {
            switch (index) {
                case 1: {
                    m_format->setText(tr("bool"));
                    break;
                }
                case 2: {
                    m_format->setText(tr("ZH;ZH;En;En"));
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
                default: {
                    m_format->setText(tr("null"));
                    break;
                }
            }
        }

        void TreeDevWidget::on_add_clicked() {
            auto itemName = m_name->text();
            auto itemEnName = m_enName->text();
            auto value = m_value->text();
            auto remark = m_remark->text();

            QStringList remarks;

            if (remark.contains(";")) {
                remarks = remark.split(";");
                if (remarks.size() != 2) {
                    auto m_box =
                        TreeJsonUtil::messageBox(tr("Warning"), tr("The remark format must be: Chinese;English"));
                    m_box->text();
                    return;
                }
            } else {
                remarks << ""
                        << "";
            }


            auto *Label = new QLabel();
            QTreeWidgetItem *child =
                new QTreeWidgetItem(QStringList() << itemName << itemEnName << "" << remarks[0] << remarks[1]);
            child->setFlags(child->flags() | Qt::ItemIsEditable);
            m_treeWidget->setItemWidget(child, 2, Label);

            QTreeWidgetItem *selectedItem = m_treeWidget->currentItem();
            if (!m_childType->currentIndex()) {
                m_treeWidget->addTopLevelItem(child);
            } else {
                if (selectedItem) {
                    selectedItem->addChild(child);
                } else {
                    auto m_box = TreeJsonUtil::messageBox(tr("Warning"), tr("Unchecked item cannot add sub items"));
                    m_box->exec();
                    return;
                }
            }

            switch (m_type->currentIndex()) {
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
                        auto m_box = TreeJsonUtil::messageBox(
                            tr("Warning"), tr("Please input\r\n\"Chinese; Chinese; English; English\" in the "
                                              "following format to ensure "
                                              "that the items are even numbers, and click addButton again."));
                        m_box->exec();
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
                case 5: {
                    auto widget = TreeJsonUtil::qFileWidget();
                    m_treeWidget->setItemWidget(child, 2, widget);
                    break;
                }
                default:
                    break;
            }
            m_treeWidget->expandAll();
            m_treeWidget->resizeColumnToContents(0);
            m_treeWidget->resizeColumnToContents(1);
        }

        void TreeDevWidget::on_remove_clicked() {
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

        // move item up
        void TreeDevWidget::on_btnUp_clicked() {
            moveItem(true);
        }

        // move item down
        void TreeDevWidget::on_btnDown_clicked() {
            moveItem(false);
        }

        void TreeDevWidget::moveItem(bool up) {
            QTreeWidgetItem *item = m_treeWidget->currentItem();
            if (item) {
                QJsonArray data;
                QTreeWidgetItem *parent = item->parent();
                auto maxIndex = parent ? parent->childCount() - 1 : m_treeWidget->topLevelItemCount() - 1;
                int index = parent ? parent->indexOfChild(item) : m_treeWidget->indexOfTopLevelItem(item);
                data.insert(0, TreeJsonUtil::JsonArrayFromTree(m_treeWidget, parent).at(index).toObject());
                if ((up && index > 0) || (!up && index < maxIndex)) {
                    auto insertIndex = up ? index - 1 : index + 1;
                    if (parent == nullptr) {
                        m_treeWidget->takeTopLevelItem(index);
                    } else {
                        parent->takeChild(index);
                    }
                    TreeJsonUtil::TreeFromJsonArray(data, insertIndex, true, m_treeWidget, parent);
                    auto cItem = parent ? parent->child(insertIndex) : m_treeWidget->topLevelItem(insertIndex);
                    m_treeWidget->setCurrentItem(cItem);
                    m_treeWidget->expandItem(cItem);
                }
            }
        }

    }
}