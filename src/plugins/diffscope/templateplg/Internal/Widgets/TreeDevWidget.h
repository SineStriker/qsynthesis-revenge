#ifndef CHORUSKIT_TREEDEVWIDGET_H
#define CHORUSKIT_TREEDEVWIDGET_H

#include "../Widgets/TreeConfigWidget.h"

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>

namespace TemplatePlg {
    namespace Internal {
        class TreeDevWidget : public QWidget {
            Q_OBJECT
            friend class TreeConfigWidget;
        private:
            TreeDevWidget(QTreeWidget *m_treeWidget, QWidget *parent = nullptr);
            ~TreeDevWidget();

            void moveItem(bool up);
            QTreeWidget *m_treeWidget;

            QLineEdit *m_name;
            QLineEdit *m_enName;
            QLabel *m_format;
            QLineEdit *m_value;
            QLineEdit *m_remark;

            QComboBox *m_childType;
            QComboBox *m_type;

            QPushButton *m_up;
            QPushButton *m_down;
            QPushButton *m_addButton;
            QPushButton *m_removeButton;

        private slots:
            void _q_formatChanged(int index);
            void _q_addClicked();
            void _q_removeClicked();
            void _q_btnUpClicked();
            void _q_btnDownClicked();
        };
    }
}

#endif // CHORUSKIT_TREEDEVWIDGET_H
