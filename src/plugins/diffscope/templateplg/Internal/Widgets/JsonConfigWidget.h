#ifndef CHORUSKIT_JSONCONFIGWIDEGT_H
#define CHORUSKIT_JSONCONFIGWIDEGT_H

#include "../Utils/ConfigModel.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QWidget>

namespace TemplatePlg {
    namespace Internal {
        class JsonConfigWidget : public QWidget {
            Q_OBJECT
        public:
            explicit JsonConfigWidget(QWidget *parent = nullptr);

        private:
            QTreeView *m_treeView;
            ConfigModel *m_model;

        private slots:
            void saveJson();
        };
    }
}


#endif // CHORUSKIT_JSONCONFIGWIDEGT_H
