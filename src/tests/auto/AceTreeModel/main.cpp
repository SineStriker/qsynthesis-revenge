#include <QDebug>
#include <QtTest/QtTest>

#include <AceTreeModel.h>

class tst_AceTreeModel : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void init();

private slots:
    void basic();
};

void tst_AceTreeModel::init() {
}

void tst_AceTreeModel::basic() {
    AceTreeModel model;

    auto rootItem = new AceTreeItem("1");
    rootItem->setProperty("key1", "value1");
    model.setRootItem(rootItem);

    rootItem->setProperty("key1", "value2");
    model.previousStep();

    QCOMPARE(rootItem->property("key1"), "value1");
}

QTEST_APPLESS_MAIN(tst_AceTreeModel)
#include "main.moc"