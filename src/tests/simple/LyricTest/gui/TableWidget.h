#ifndef CHORUSKIT_TABLEWIDGET_H
#define CHORUSKIT_TABLEWIDGET_H

#include <QHash>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QWidget>

class TableWidget : public QWidget {
    Q_OBJECT
public:
    explicit TableWidget(QWidget *parent = nullptr);
    ~TableWidget();

protected:
    QTableWidget *m_tableWidget;
    QPushButton *m_saveButton;
    QHash<QString, QString> m_hash;

private:
    void addTableRow();
    void deleteTableRow();
    void saveTableRows();
};


#endif // CHORUSKIT_TABLEWIDGET_H
