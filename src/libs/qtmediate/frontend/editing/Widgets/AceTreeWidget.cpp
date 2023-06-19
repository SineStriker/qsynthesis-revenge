#include "AceTreeWidget.h"
#include "AceTreeWidget_p.h"

#include <QDebug>
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QStack>

AceTreeWidgetPrivate::AceTreeWidgetPrivate() {
}

AceTreeWidgetPrivate::~AceTreeWidgetPrivate() {
}

void AceTreeWidgetPrivate::init() {
    Q_Q(AceTreeWidget);

    // q->setHeaderHidden(true);
    // q->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    q->setColumnCount(3);

    m_model = new AceTreeModel(this);
    connect(m_model, &AceTreeModel::propertyChanged, this, &AceTreeWidgetPrivate::_q_propertyChanged);
    connect(m_model, &AceTreeModel::rowsInserted, this, &AceTreeWidgetPrivate::_q_rowsInserted);
    connect(m_model, &AceTreeModel::rowsMoved, this, &AceTreeWidgetPrivate::_q_rowsMoved);
    connect(m_model, &AceTreeModel::rowsAboutToRemove, this, &AceTreeWidgetPrivate::_q_rowsAboutToRemove);
    connect(m_model, &AceTreeModel::rowsRemoved, this, &AceTreeWidgetPrivate::_q_rowsRemoved);
    connect(m_model, &AceTreeModel::nodeAdded, this, &AceTreeWidgetPrivate::_q_nodeAdded);
    connect(m_model, &AceTreeModel::nodeAboutToRemove, this, &AceTreeWidgetPrivate::_q_nodeAboutToRemove);
    connect(m_model, &AceTreeModel::nodeRemoved, this, &AceTreeWidgetPrivate::_q_nodeRemoved);
    connect(m_model, &AceTreeModel::rootAboutToChange, this, &AceTreeWidgetPrivate::_q_rootAboutToChange);
    connect(m_model, &AceTreeModel::rootChanged, this, &AceTreeWidgetPrivate::_q_rootChanged);

    connect(q, &CTreeWidget::itemClickedEx, this, &AceTreeWidgetPrivate::_q_itemClickedEx);
}

QTreeWidgetItem *AceTreeWidgetPrivate::buildItem(AceTreeItem *item) {
    auto res = new QTreeWidgetItem();
    itemIndexes.insert(item, res);

    res->setData(0, Qt::UserRole + 1, QVariant::fromValue((uintptr_t) item));
    res->setText(0, item->name());
    res->setText(1, item->property("text").toString());
    res->setText(2, item->bytes().toHex());

    auto vectorItem = new QTreeWidgetItem();
    vectorItem->setData(0, Qt::UserRole + 2, "vector");
    vectorItem->setText(0, "Vector");
    vectorItem->setText(1, QString::number(item->rowCount()));
    for (int i = 0; i < item->rowCount(); ++i) {
        vectorItem->addChild(buildItem(item->row(i)));
    }

    auto setItem = new QTreeWidgetItem();
    setItem->setData(0, Qt::UserRole + 2, "set");
    setItem->setText(0, "Set");
    setItem->setText(1, QString::number(item->nodeCount()));
    for (const auto &child : item->nodes()) {
        setItem->addChild(buildItem(child));
    }

    res->addChild(vectorItem);
    res->addChild(setItem);

    return res;
}

void AceTreeWidgetPrivate::removeItem(AceTreeItem *item) {
    auto treeItem = itemIndexes.value(item, nullptr);
    if (!treeItem)
        return;

    QStack<AceTreeItem *> stack;
    stack.push(item);
    while (stack.isEmpty()) {
        auto top = stack.pop();
        itemIndexes.remove(top);
        for (int i = 0; i < item->rowCount(); ++i) {
            stack.push(item->row(i));
        }
        for (const auto &child : item->nodes()) {
            stack.push(child);
        }
    }

    delete treeItem;
}

void AceTreeWidgetPrivate::_q_propertyChanged(AceTreeItem *item, const QString &key, const QVariant &newValue,
                                              const QVariant &oldValue) {
    if (key == "text") {
        auto treeItem = itemIndexes.value(item, nullptr);
        if (treeItem) {
            treeItem->setText(1, newValue.toString());
        }
    }
}

void AceTreeWidgetPrivate::_q_rowsInserted(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items) {
    auto parentItem = itemIndexes.value(parent, nullptr);
    if (!parentItem) {
        return;
    }

    QList<QTreeWidgetItem *> treeItems;
    treeItems.reserve(items.size());
    for (const auto &item : items)
        treeItems.append(buildItem(item));

    auto vectorItem = parentItem->child(0);
    vectorItem->setText(1, QString::number(parent->rowCount()));
    vectorItem->insertChildren(index, treeItems);
}

void AceTreeWidgetPrivate::_q_rowsMoved(AceTreeItem *parent, int index, int count, int dest) {
    auto parentItem = itemIndexes.value(parent, nullptr);
    if (!parentItem) {
        return;
    }

    auto vectorItem = parentItem->child(0);
    vectorItem->takeChildren();
    for (int i = 0; i < parent->rowCount(); ++i) {
        vectorItem->addChild(itemIndexes.value(parent->row(i)));
    }
}

void AceTreeWidgetPrivate::_q_rowsAboutToRemove(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items) {
    Q_Q(AceTreeWidget);
}

void AceTreeWidgetPrivate::_q_rowsRemoved(AceTreeItem *parent, int index, const QVector<AceTreeItem *> &items) {
    auto parentItem = itemIndexes.value(parent, nullptr);
    if (!parentItem) {
        return;
    }

    auto vectorItem = parentItem->child(0);

    QList<QTreeWidgetItem *> childItems;
    for (int i = 0; i < items.size(); ++i) {
        auto childItem = vectorItem->child(index + i);
        childItems.append(childItem);
    }
    for (const auto &item : qAsConst(childItems)) {
        auto item1 = reinterpret_cast<AceTreeItem *>(item->data(0, Qt::UserRole + 1).value<uintptr_t>());
        removeItem(item1);
    }

    vectorItem->setText(1, QString::number(parent->rowCount()));
}

void AceTreeWidgetPrivate::_q_nodeAdded(AceTreeItem *parent, AceTreeItem *item) {
    auto parentItem = itemIndexes.value(parent, nullptr);
    if (!parentItem) {
        return;
    }

    auto setItem = parentItem->child(1);
    setItem->setText(1, QString::number(parent->nodeCount()));
    setItem->addChild(buildItem(item));
}

void AceTreeWidgetPrivate::_q_nodeAboutToRemove(AceTreeItem *parent, AceTreeItem *item) {
}

void AceTreeWidgetPrivate::_q_nodeRemoved(AceTreeItem *parent, AceTreeItem *item) {
    auto parentItem = itemIndexes.value(parent, nullptr);
    if (!parentItem) {
        return;
    }

    removeItem(item);

    auto setItem = parentItem->child(1);
    setItem->setText(1, QString::number(parent->nodeCount()));
}

void AceTreeWidgetPrivate::_q_rootAboutToChange(AceTreeItem *newRoot, AceTreeItem *oldRoot) {
    removeItem(oldRoot);
}

void AceTreeWidgetPrivate::_q_rootChanged(AceTreeItem *newRoot, AceTreeItem *oldRoot) {
    Q_Q(AceTreeWidget);
    auto rootItem = m_model->rootItem();
    if (!rootItem)
        return;
    auto item = buildItem(rootItem);
    q->addTopLevelItem(item);
}

void AceTreeWidgetPrivate::_q_itemClickedEx(QTreeWidgetItem *item, int column, Qt::MouseButton button) {
    Q_Q(AceTreeWidget);

    Q_UNUSED(column);

    if (!item)
        return;

    auto addItem = [q]() -> AceTreeItem * {
        QInputDialog dlg(q);
        dlg.setInputMode(QInputDialog::TextInput);
        dlg.setLabelText("Name");

        QString text;
        while (text.isEmpty()) {
            if (dlg.exec() == QDialog::Accepted) {
                text = dlg.textValue();
                continue;
            }
            return nullptr;
        }

        return new AceTreeItem(text);
    };

    if (button == Qt::LeftButton) {
        auto typeData = item->data(0, Qt::UserRole + 2).toString();
        if (typeData == "vector") {
            auto parentItem1 =
                reinterpret_cast<AceTreeItem *>(item->parent()->data(0, Qt::UserRole + 1).value<uintptr_t>());
            parentItem1->appendRow(addItem());
        } else if (typeData == "set") {
            auto parentItem1 =
                reinterpret_cast<AceTreeItem *>(item->parent()->data(0, Qt::UserRole + 1).value<uintptr_t>());
            parentItem1->addNode(addItem());
        }
    } else if (button == Qt::RightButton) {
        auto parentItem = item->parent();
        bool isRow = parentItem ? parentItem->data(0, Qt::UserRole + 2).toString() == "vector" : false;
        bool isNode = (parentItem && !isRow) ? parentItem->data(0, Qt::UserRole + 2).toString() == "set" : false;

        if (!isRow && !isNode && parentItem) {
            return;
        }

        QMenu menu;

        auto modifyText = menu.addAction("Modify text");
        auto remove = menu.addAction("Remove");
        auto insertBefore = isRow ? menu.addAction("Insert before") : nullptr;
        auto insertAfter = isRow ? menu.addAction("Insert after") : nullptr;
        auto insetNode = (isRow && isNode) ? menu.addAction("Insert") : nullptr;

        auto item1 = reinterpret_cast<AceTreeItem *>(item->data(0, Qt::UserRole + 1).value<uintptr_t>());
        auto parentItem1 = item1->parent();

        auto action = menu.exec(QCursor::pos());
        if (modifyText && action == modifyText) {
            QInputDialog dlg(q);
            dlg.setInputMode(QInputDialog::TextInput);
            dlg.setLabelText("Text");
            int code = dlg.exec();
            if (code == QDialog::Accepted) {
                auto text = dlg.textValue();
                item1->setProperty("text", text);
            }
        } else if (remove && action == remove) {
            if (isRow) {
                parentItem1->removeRow(parentItem1->rowIndexOf(item1));
            } else if (isNode) {
                parentItem1->removeNode(item1);
            } else {
                m_model->setRootItem(nullptr);
            }
        } else if (insertBefore && action == insertBefore) {
            parentItem1->insertRow(parentItem1->rowIndexOf(item1), addItem());
        } else if (insertAfter && action == insertAfter) {
            parentItem1->insertRow(parentItem1->rowIndexOf(item1) + 1, addItem());
        } else if (insetNode && action == insetNode) {
            parentItem1->insertNode(addItem());
        }
    } else if (button == Qt::MiddleButton) {
        auto typeData = item->data(0, Qt::UserRole + 2).toString();
        if (typeData != "vector") {
            return;
        }
        auto parentItem1 = reinterpret_cast<AceTreeItem *>(item->parent()->data(0, Qt::UserRole + 1).value<uintptr_t>());
        QInputDialog dlg(q);
        dlg.setInputMode(QInputDialog::TextInput);
        dlg.setLabelText("Range(format: idx cnt dest)");

        int idx = -1;
        int cnt = 0;
        int dest = -1;
        while (idx < 0 || cnt == 0 || dest < 0) {
            if (dlg.exec() == QDialog::Accepted) {
                QStringList texts = dlg.textValue().split(" ");
                if (texts.size() >= 3) {
                    idx = texts.at(0).toInt();
                    cnt = texts.at(1).toInt();
                    dest = texts.at(2).toInt();
                }
                continue;
            }
            return;
        }

        parentItem1->moveRows(idx, cnt, dest);
    }
}

AceTreeWidget::AceTreeWidget(QWidget *parent) : AceTreeWidget(*new AceTreeWidgetPrivate(), parent) {
}

AceTreeWidget::~AceTreeWidget() {
}

AceTreeModel *AceTreeWidget::model() const {
    Q_D(const AceTreeWidget);
    return d->m_model;
}

AceTreeWidget::AceTreeWidget(AceTreeWidgetPrivate &d, QWidget *parent) : CTreeWidget(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
