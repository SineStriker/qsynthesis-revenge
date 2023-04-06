//
// Created by Crs_1 on 2023/4/5.
//

#ifndef CHORUSKIT_PREFERENCEREGISTRY_P_H
#define CHORUSKIT_PREFERENCEREGISTRY_P_H
#include "PreferenceRegistry.h"
#include <QLabel>
#include <QMap>
#include <QSet>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QLineEdit>


namespace Core {
    class PreferenceRegistryPrivate: public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(PreferenceRegistry)
    public:
        PreferenceRegistry *q_ptr;

        class Node;

        class MyTreeWidgetItem: public QTreeWidgetItem {
        public:
            Node *node();
        private:
            friend class Node;
            Node *m_node;
            void setNode(Node *node);
        };

        class Node: public QObject {
            Q_OBJECT
        public:
            explicit Node(IPreferenceItem *item, Node *parent, std::set<Node *> children);
            ~Node();
            IPreferenceItem *item;
            Node *parent;
            std::set<Node *> children;
            MyTreeWidgetItem *widgetItem;
            bool operator< (const Node &rhs);
        };
        void addNode(Node *node);
        void removeNode(const QString &id);
        Node *getNode(const QString &id);
        void repaintNode(Node *node);
        Node *root;
        QMap<QString, Node *> nodeList;
        QSet<IPreferenceItem *> itemList;

        void commitWidgetUpdate();

        class PreferencePage: public IDialogPage {
            Q_OBJECT
        public:
            ~PreferencePage();
            QString id() const override;
            QString title() const override;
            void reloadStrings() override;
            QWidget *widget() override;
        public slots:
            void load() override;
            void accept() override;
            void finish() override;
            void setPage(IOptionsPage *page);
        private:
            friend class PreferenceRegistryPrivate;
            friend class PreferenceRegistry;
            QWidget *m_widget = nullptr;
            QLineEdit *m_filterEdit = nullptr;
            QTreeWidget *m_treeWidget = nullptr;
            QLabel *m_titleLabel = nullptr;
            QLabel *m_descriptionLabel = nullptr;
            QStackedWidget *m_stackedWidget = nullptr;

        };
        PreferencePage *preferencePage;
        void repaintPreferencePage();
    signals:
        void reloadStrings();
    private:
        bool lazyWidgetUpdate = true;
    };
}

#endif // CHORUSKIT_PREFERENCEREGISTRY_P_H
