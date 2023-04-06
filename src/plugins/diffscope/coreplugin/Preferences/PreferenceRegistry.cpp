//
// Created by Crs_1 on 2023/4/5.
//

#include "PreferenceRegistry.h"

#include "ICore.h"
#include "IOptionsPage.h"
#include "PreferenceRegistry_p.h"
#include "QMDecorator.h"
#include "RegistryNode.h"
#include <QHBoxLayout>
#include <utility>

using namespace Core;

static PreferenceRegistry *m_instance = nullptr;

void PreferenceRegistryPrivate::addNode(RegistryNode *node) {
    nodeList.insert(node->item->id(), node);
    itemList.insert(node->item);
    node->parent->children.insert(node);
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
    if(node->item && node->item->nodeType() == IPreferenceItem::Page) {
        disconnect(this, nullptr, node->item->asPage(), nullptr);
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
    d->root = new RegistryNode(nullptr, nullptr, {});
    d->nodeList.insert("", d->root);
    d->preferencePage = new PreferencePage;
    d->preferencePage->widget();
    connect(d->preferencePage->m_treeWidget, &QTreeWidget::currentItemChanged, d->preferencePage, [=](QTreeWidgetItem *current, QTreeWidgetItem *previous){
        auto currentNode = dynamic_cast<RegistryNodeTreeWidgetItem *>(current)->node();
        if(currentNode->item && currentNode->item->nodeType() == IPreferenceItem::Page) {
            d->preferencePage->setPage(currentNode->item->asPage());
        }
    });
    connect(d->preferencePage->m_filterEdit, &QLineEdit::textChanged, d, &PreferenceRegistryPrivate::filter);
    qIDec->installLocale(this, {{}}, _LOC(PreferenceRegistryPrivate, d));
}

PreferenceRegistry::PreferenceRegistry(QObject *parent): PreferenceRegistry(new PreferenceRegistryPrivate, parent) {}

RegistryNode *PreferenceRegistryPrivate::getNode(const QString &id) {
    auto it = nodeList.find(id);
    if(it == nodeList.end()) {
        return nullptr;
    }
    return it.value();
}

PreferenceRegistry::~PreferenceRegistry() noexcept {
    Q_D(PreferenceRegistry);
    for(auto node: d->nodeList) {
        node->deleteLater();
    }
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
    auto node = new RegistryNode(item, parentNode, {});
    connect(d, &PreferenceRegistryPrivate::reloadStrings, node, &RegistryNode::reloadStrings);
    if(item->nodeType() == IPreferenceItem::Page) {
        connect(d->preferencePage, &IDialogPage::accepted, item->asPage(), [=](){
            item->asPage()->accept();
            emit item->asPage()->accepted();
        });
        connect(d->preferencePage, &IDialogPage::done, item->asPage(), [=](int result){
            item->asPage()->finish();
            emit item->asPage()->done(result);
        });
    }
    d->addNode(node);
    d->commitWidgetUpdate();
}

void PreferenceRegistry::removeItem(const QString &id) {
    Q_D(PreferenceRegistry);
    d->removeNode(id);
    d->commitWidgetUpdate();
}

void PreferenceRegistryPrivate::repaintNode(RegistryNode *node) {
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
    preferencePage->widget();
    for(int i = 0; i < preferencePage->m_treeWidget->topLevelItemCount(); i++) {
        preferencePage->m_treeWidget->takeTopLevelItem(i);
    }
    for(auto node: root->children) {
        repaintNode(node);
        preferencePage->m_treeWidget->addTopLevelItem(node->widgetItem);
    }
    while(preferencePage->m_stackedWidget->count() > 0) {
        preferencePage->m_stackedWidget->removeWidget(preferencePage->m_stackedWidget->widget(0));
    }
    for(auto node: nodeList) {
        if(node->item && node->item->nodeType() == IPreferenceItem::Page) {
            preferencePage->m_stackedWidget->addWidget(node->item->asPage()->widget());
            node->item->asPage()->reloadStrings();
        }
    }
    lazyWidgetUpdate = false;
}

void PreferenceRegistry::showPreferenceDialog(QWidget *parent, const QString &entryPageId) {
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
    d->preferencePage->setPage(currentNode->item->asPage());
    for(auto node: d->nodeList) {
        if(node->item && node->item->nodeType() == IPreferenceItem::Page) {
            node->item->asPage()->load();
        }
    }
    ICore::instance()->dialogHelper()->showDialog(d->preferencePage->id(), parent, DialogHelper::OkButton | DialogHelper::CancelButton | DialogHelper::ApplyButton | DialogHelper::CloseButton);
}

void PreferenceRegistry::closePreferenceDialog(int result) {
    Q_D(PreferenceRegistry);
    ICore::instance()->dialogHelper()->closeDialog(d->preferencePage->id(), result);
}

void PreferenceRegistryPrivate::filter(const QString &pattern) {
    for(auto node: nodeList) {
        if(node->item && node->item->nodeType() == IPreferenceItem::Page) {
            if(node->item->asPage()->match(pattern)) {
                //TODO do something (e. g. highlight the title)
            }
        }
    }
}
