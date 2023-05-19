#ifndef CHORUSKIT_TREECONFIGWIDGET_H
#define CHORUSKIT_TREECONFIGWIDGET_H

#include "../Utils/TreeJsonUtil.h"

#include <QHBoxLayout>
#include <QJsonObject>
#include <QPushButton>
#include <QTreeWidget>

namespace TemplatePlg {
    namespace Internal {
        class TreeConfigWidget : public QWidget {
            Q_OBJECT
        public:
            explicit TreeConfigWidget(const QString &pluginId, const QString &configDir, bool configGen = false,
                                      QWidget *parent = nullptr);
            ~TreeConfigWidget();

            static TreeConfigWidget *instance(const QString &pluginId);
            QWidget *configWidget();
            QVariant readConfig(const QString &path, QString type = "index");

        protected:
            QString pluginId;
            QString uiPath;
            QString configPath;
            QString developUiPath;
            QJsonObject configModel;
            bool configGen;
            QString m_language;

            QWidget *m_widget;
            QTreeWidget *m_treeWidget;
            QVBoxLayout *mainLayout;

            QPushButton *m_defaultButton;
            QPushButton *m_loadButton;
            QPushButton *m_saveButton;

        private:
            QWidget *createWidget();
            QHBoxLayout *treeWidgetBox();
            QHBoxLayout *bottomButtonBox();
            void loadConfig(const QJsonObject &configObj, QTreeWidget *treeWidget, QTreeWidgetItem *item = nullptr);

        private slots:
            bool _q_saveClicked();
            void _q_defaultClicked();
            void _q_loadConfigClicked();
        };

    }
}
#endif // CHORUSKIT_TREECONFIGWIDGET_H
