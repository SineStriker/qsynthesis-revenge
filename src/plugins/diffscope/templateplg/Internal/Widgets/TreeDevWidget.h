#ifndef CHORUSKIT_TREEDEVWIDGET_H
#define CHORUSKIT_TREEDEVWIDGET_H

#include "QPushButton"
#include "QTreeWidget"
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QLocale>
#include <QMessageBox>
#include <QSpinBox>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace TemplatePlg {
    namespace Internal {
        class TreeDevWidget : public QWidget {
            Q_OBJECT
        public:
            TreeDevWidget(QTreeWidget *m_treeWidget, QWidget *parent = nullptr);
            ~TreeDevWidget();

        private:
            void moveItem(bool up);
            QTreeWidget *m_treeWidget;
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

        private slots:
            void on_format_Changed(int index);
            void on_add_clicked();
            void on_remove_clicked();
            void on_btnUp_clicked();
            void on_btnDown_clicked();
        };
    }
}

#endif // CHORUSKIT_TREEDEVWIDGET_H
