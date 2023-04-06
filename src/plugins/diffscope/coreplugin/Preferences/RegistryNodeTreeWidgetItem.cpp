//
// Created by Crs_1 on 2023/4/6.
//

#include "RegistryNodeTreeWidgetItem.h"

using namespace Core;

RegistryNode *RegistryNodeTreeWidgetItem::node() {
    return m_node;
}

void RegistryNodeTreeWidgetItem::setNode(RegistryNode *node) {
    m_node = node;
}