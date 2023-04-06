//
// Created by Crs_1 on 2023/4/5.
//

#include "PreferenceRegistry.h"

#include "ICore.h"
#include "IOptionsPage.h"
#include "PreferenceRegistry_p.h"
#include <QHBoxLayout>
#include <utility>

using namespace Core;

static PreferenceRegistry *m_instance = nullptr;

PreferenceRegistryPrivate::Node *PreferenceRegistryPrivate::MyTreeWidgetItem::node() {
    return m_node;
}

void PreferenceRegistryPrivate::MyTreeWidgetItem::setNode(Node *node) {
    m_node = node;
}

PreferenceRegistryPrivate::Node::Node(IPreferenceItem *item, Node *parent, std::set<Node *> children) {
    this->item = item;
    this->parent = parent;
    this->children = std::move(children);
    this->widgetItem = new PreferenceRegistryPrivate::MyTreeWidgetItem;
    widgetItem->setNode(this);
    widgetItem->setText(0, item->title());
    widgetItem->setToolTip(0, item->description());
}

PreferenceRegistryPrivate::Node::~Node() {
    if(item->nodeType() == IPreferenceItem::Page) {
        dynamic_cast<IOptionsPage *>(item)->deleteLater();
    } else {
        delete item;
    }
    for(auto child: children) {
        delete child;
    }
}

bool PreferenceRegistryPrivate::Node::operator<(const Node &rhs) {
    //TODO locale string sort
    return item->title() < rhs.item->title();
}

void PreferenceRegistryPrivate::addNode(Node *node) {
    nodeList.insert(node->item->id(), node);
    itemList.insert(node->item);
}

void PreferenceRegistryPrivate::removeNode(const QString &id) {
    auto node = getNode(id);
    if(!node) {
        //TODO warning not found
        return;
    }
    if(node->parent) {
        node->parent->children.erase(node);
    }
    nodeList.erase(nodeList.find(id));
    itemList.erase(itemList.find(node->item));
    disconnect(this, &PreferenceRegistryPrivate::reloadStrings, node, nullptr);
    if(node->item->nodeType() == IPreferenceItem::Page) {
        disconnect(this, &PreferenceRegistryPrivate::reloadStrings, dynamic_cast<IOptionsPage *>(node->item), nullptr);
    }
    for(auto child: node->children) {
        removeNode(child->item->id());
    }
    node->deleteLater();
}

void PreferenceRegistryPrivate::commitWidgetUpdate() {
    lazyWidgetUpdate = true;
}

PreferenceRegistry::PreferenceRegistry(PreferenceRegistryPrivate *d, QObject *parent): QObject(parent), d_ptr(d) {
    m_instance = this;
    d->q_ptr = this;
    d->root = new PreferenceRegistryPrivate::Node(nullptr, nullptr, {});
    d->nodeList.insert("", d->root);
    d->preferencePage = new PreferenceRegistryPrivate::PreferencePage;
    connect(d->preferencePage->m_treeWidget, &QTreeWidget::currentItemChanged, d->preferencePage, [=](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        auto currentNode = dynamic_cast<PreferenceRegistryPrivate::MyTreeWidgetItem *>(current)->node();
        if(currentNode->item->nodeType() == IPreferenceItem::Page) {
            d->preferencePage->setPage(dynamic_cast<IOptionsPage *>(currentNode->item));
        }
    });
}

PreferenceRegistry::PreferenceRegistry(QObject *parent): PreferenceRegistry(new PreferenceRegistryPrivate, parent) {}

PreferenceRegistryPrivate::Node *PreferenceRegistryPrivate::getNode(const QString &id) {
    auto it = nodeList.find(id);
    if(it == nodeList.end()) {
        return nullptr;
    }
    return it.value();
}

IPreferenceItem *PreferenceRegistry::getItem(const QString &id) {
    Q_D(PreferenceRegistry);
    auto node = d->getNode(id);
    if(!node) {
        return nullptr;
    }
    return node->item;
}

void PreferenceRegistry::addItem(IPreferenceItem *item, const QString &parentId) {
    Q_D(PreferenceRegistry);
    auto parentNode = d->getNode(parentId);
    if(!parentNode) {
        //TODO warning parent not found
        return;
    }
    if(d->itemList.contains(item)) {
        //TODO warning item already added
        return;
    }
    auto node = new PreferenceRegistryPrivate::Node(item, parentNode, {});
    connect(d, &PreferenceRegistryPrivate::reloadStrings, node, [=](){
        node->widgetItem->setText(0, node->item->title());
        node->widgetItem->setToolTip(0, node->item->description());
        if(node->item->nodeType() == IPreferenceItem::Page) {
            dynamic_cast<IOptionsPage *>(node->item)->reloadStrings();
        }
    });
    d->addNode(node);
    d->commitWidgetUpdate();
}

void PreferenceRegistry::removeItem(const QString &id) {
    Q_D(PreferenceRegistry);
    d->removeNode(id);
}

QString PreferenceRegistryPrivate::PreferencePage::id() const {
    return "ChorusKit:Preference";
}

QString PreferenceRegistryPrivate::PreferencePage::title() const {
    return tr("Preference");
}

void PreferenceRegistryPrivate::PreferencePage::reloadStrings() {
    //TODO
}

QWidget *PreferenceRegistryPrivate::PreferencePage::widget() {
    if(!m_widget) {
        m_widget = new QWidget;
        auto mainLayout = new QHBoxLayout;
        auto selectAreaLayout = new QVBoxLayout;
        auto optionsAreaLayout = new QVBoxLayout;
        m_treeWidget = new QTreeWidget;
        m_filterEdit = new QLineEdit;
        m_titleLabel = new QLabel;
        m_descriptionLabel = new QLabel;
        m_stackedWidget = new QStackedWidget;
        selectAreaLayout->addWidget(m_filterEdit);
        selectAreaLayout->addWidget(m_treeWidget);
        optionsAreaLayout->addWidget(m_titleLabel);
        optionsAreaLayout->addWidget(m_descriptionLabel);
        optionsAreaLayout->addWidget(m_stackedWidget);
        mainLayout->addLayout(selectAreaLayout);
        mainLayout->addLayout(optionsAreaLayout);
        m_widget->setLayout(mainLayout);
    }
    return m_widget;
}

void PreferenceRegistryPrivate::repaintNode(Node *node) {
    for(int i = 0; i < node->widgetItem->childCount(); i++) {
        node->widgetItem->takeChild(i);
    }
    for(auto child: node->children) {
        repaintNode(child);
        node->widgetItem->addChild(child->widgetItem);
    }
}

void PreferenceRegistryPrivate::repaintPreferencePage() {
    if(!lazyWidgetUpdate) return;
    for(int i = 0; i < preferencePage->m_treeWidget->topLevelItemCount(); i++) {
        preferencePage->m_treeWidget->takeTopLevelItem(i);
    }
    for(auto node: root->children) {
        repaintNode(node);
        preferencePage->m_treeWidget->addTopLevelItem(node->widgetItem);
    }
    lazyWidgetUpdate = false;
}

void PreferenceRegistry::showPreferenceDialog(QWidget *parent, QString &entryPageId) {
    Q_D(PreferenceRegistry);
    if(!ICore::instance()->dialogHelper()->getDialogPage(d->preferencePage->id())) {
        ICore::instance()->dialogHelper()->addDialogPage(d->preferencePage);
    }
    d->repaintPreferencePage();
    auto currentNode = d->getNode(entryPageId);
    if(!currentNode || currentNode->item->nodeType() != IPreferenceItem::Page) {
        //TODO warning invalid entry page
        return;
    }
    
    d->preferencePage->m_treeWidget->setCurrentItem(currentNode->widgetItem);
}
