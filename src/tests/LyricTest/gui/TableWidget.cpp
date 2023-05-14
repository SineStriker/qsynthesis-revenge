#include "TableWidget.h"



TableWidget::TableWidget(QWidget *parent) : QWidget(parent) {
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(2);
    m_tableWidget->setHorizontalHeaderLabels({"多音词", "拼音"});

    m_saveButton = new QPushButton("Save", this);
    connect(m_saveButton, &QPushButton::clicked, this, &TableWidget::saveTableRows);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_tableWidget);
    layout->addWidget(m_saveButton);

    QPushButton *addButton = new QPushButton("Add Row", this);
    connect(addButton, &QPushButton::clicked, this, &TableWidget::addTableRow);
    layout->addWidget(addButton);

    QPushButton *deleteButton = new QPushButton("Delete Row", this);
    connect(deleteButton, &QPushButton::clicked, this, &TableWidget::deleteTableRow);
    layout->addWidget(deleteButton);
    setLayout(layout);
}

TableWidget::~TableWidget() {
}

void TableWidget::addTableRow() {
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    QTableWidgetItem *item1 = new QTableWidgetItem("Column 1");
    QTableWidgetItem *item2 = new QTableWidgetItem("Column 2");

    m_tableWidget->setItem(row, 0, item1);
    m_tableWidget->setItem(row, 1, item2);
}

void TableWidget::deleteTableRow() {
    int row = m_tableWidget->currentRow();
    m_tableWidget->removeRow(row);
}

void TableWidget::saveTableRows() {
    m_hash.clear();

    for (int row = 0; row < m_tableWidget->rowCount(); row++) {
        QTableWidgetItem *item1 = m_tableWidget->item(row, 0);
        QTableWidgetItem *item2 = m_tableWidget->item(row, 1);

        if (item1 && item2) {
            m_hash.insert(item1->text(), item2->text());
        }
    }
}