//
// Created by Crs_1 on 2023/4/6.
//

#ifndef CHORUSKIT_REGISTRYNODETREEWIDGETITEM_H
#define CHORUSKIT_REGISTRYNODETREEWIDGETITEM_H

#include <QTreeWidgetItem>

namespace Core {

    class RegistryNode;
    class RegistryNodeTreeWidgetItem : public QTreeWidgetItem {
    public:
        RegistryNode *node();
    private:
        friend class RegistryNode;
        RegistryNode *m_node;
        void setNode(RegistryNode *node);
    };
}



#endif // CHORUSKIT_REGISTRYNODETREEWIDGETITEM_H
