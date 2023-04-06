//
// Created by Crs_1 on 2023/4/6.
//

#ifndef CHORUSKIT_REGISTRYNODE_H
#define CHORUSKIT_REGISTRYNODE_H



#include "IPreferenceItem.h"
#include "RegistryNodeTreeWidgetItem.h"
#include <set>

namespace Core {
    class RegistryNode : public QObject {
        Q_OBJECT
    public:
        explicit RegistryNode(IPreferenceItem *item, RegistryNode *parent, std::set<RegistryNode *> children);
        ~RegistryNode();
        IPreferenceItem *item;
        RegistryNode *parent;
        std::set<RegistryNode *> children;
        RegistryNodeTreeWidgetItem *widgetItem;
        bool operator< (const RegistryNode &rhs);
    public slots:
        void reloadStrings();
    };
}



#endif // CHORUSKIT_REGISTRYNODE_H
