#ifndef CHORUSKIT_TREECONFIGWIDGET_H
#define CHORUSKIT_TREECONFIGWIDGET_H

#include "../Utils/TreeJsonUtil.h"
#include "QPushButton"
#include "QTreeWidget"
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>

namespace TemplatePlg {
    namespace Internal {
        class TreeConfigWidget : public QWidget {
            Q_OBJECT
        public:
            explicit TreeConfigWidget(QString configDir, bool configGen = false, QWidget *parent = nullptr);
            ~TreeConfigWidget();
            static TreeConfigWidget *Instance();
            QWidget *configWidget();
            QVariant readConfig(const QString path, QString type = "index");

        protected:
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
            QVBoxLayout *developButtonBox();
            QHBoxLayout *bottomButtonBox();
            QString getLocalLanguage();
            void loadConfig(const QJsonObject configObj, QTreeWidget *treeWidget, QTreeWidgetItem *item = nullptr);

        private slots:
            bool on_save_clicked();
            void on_default_clicked();
            void on_loadConfig_clicked();
        };

    }
}
#endif // CHORUSKIT_TREECONFIGWIDGET_H
