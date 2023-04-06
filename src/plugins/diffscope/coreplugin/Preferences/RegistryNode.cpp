//
// Created by Crs_1 on 2023/4/6.
//

#include "RegistryNode.h"
#include "IOptionsPage.h"

using namespace Core;

RegistryNode::RegistryNode(IPreferenceItem *item, RegistryNode *parent, std::set<RegistryNode *> children) {
    this->item = item;
    this->parent = parent;
    this->children = std::move(children);
    if(item) {
        this->widgetItem = new RegistryNodeTreeWidgetItem;
        widgetItem->setNode(this);
        widgetItem->setText(0, item->title());
        widgetItem->setToolTip(0, item->description());
    } else {
        widgetItem = nullptr;
    }

}

RegistryNode::~RegistryNode() {
    if(item && item->nodeType() == IPreferenceItem::Page) {
        dynamic_cast<IOptionsPage *>(item)->deleteLater();
    } else {
        delete item;
    }
    for(auto child: children) {
        delete child;
    }
}

bool RegistryNode::operator<(const RegistryNode &rhs) {
    //TODO locale string sort
    return item->title() < rhs.item->title();
}

