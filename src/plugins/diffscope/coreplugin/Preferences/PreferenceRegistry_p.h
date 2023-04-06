//
// Created by Crs_1 on 2023/4/5.
//

#ifndef CHORUSKIT_PREFERENCEREGISTRY_P_H
#define CHORUSKIT_PREFERENCEREGISTRY_P_H
#include "PreferencePage.h"
#include "PreferenceRegistry.h"
#include "RegistryNode.h"
#include <QMap>
#include <QSet>


namespace Core {
    class PreferenceRegistryPrivate: public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(PreferenceRegistry)
    public:
        PreferenceRegistry *q_ptr;

        void addNode(RegistryNode *node);
        void removeNode(const QString &id);
        RegistryNode *getNode(const QString &id);
        void repaintNode(RegistryNode *node);
        RegistryNode *root;
        QMap<QString, RegistryNode *> nodeList;
        QSet<IPreferenceItem *> itemList;

        void commitWidgetUpdate();
        PreferencePage *preferencePage;
        void repaintPreferencePage();
    public slots:
        void filter(const QString &pattern);
    signals:
        void reloadStrings();
    private:
        bool lazyWidgetUpdate = true;
    };
}

#endif // CHORUSKIT_PREFERENCEREGISTRY_P_H
