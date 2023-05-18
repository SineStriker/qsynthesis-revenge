#ifndef CHORUSKIT_TREECONFIGWIDGET_H
#define CHORUSKIT_TREECONFIGWIDGET_H

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
            QLineEdit *m_name;
            QLineEdit *m_enName;
            QLabel *m_format;
            QLineEdit *m_value;
            QComboBox *m_childType;
            QComboBox *m_type;
            QPushButton *m_up;
            QPushButton *m_down;
            QPushButton *m_addButton;
            QPushButton *m_removeButton;

            QPushButton *m_defaultButton;
            QPushButton *m_loadButton;
            QPushButton *m_saveButton;

        private:
            QWidget *createWidget();
            QHBoxLayout *treeWidgetBox();
            QVBoxLayout *developButtonBox();
            QHBoxLayout *bottomButtonBox();
            QString getLocalLanguage();
            QMessageBox *messageBox(QString title, QString text);
            QJsonArray readJsonFile(QString filePath);
            bool saveJsonFile(QString filePath, QJsonArray configJson);
            QJsonArray createJsonFromTree(QTreeWidget *treeWidget, QTreeWidgetItem *item = nullptr);
            QJsonObject JsonArrayToJsonObject(const QJsonArray &jsonArray);
            void loadConfig(const QJsonObject configObj, QTreeWidget *treeWidget, QTreeWidgetItem *item = nullptr);
            void insertUi(const QJsonArray &config, int insertIndex = -1, QTreeWidgetItem *parent = nullptr);

        private slots:
            void on_format_Changed(int index);
            void addTableRow();
            void removeTableRow();
            bool on_save_clicked();
            void on_btnUp_clicked();
            void on_btnDown_clicked();
            void on_default_clicked();
            void on_loadConfig_clicked();
        };

    }
}
#endif // CHORUSKIT_TREECONFIGWIDGET_H
