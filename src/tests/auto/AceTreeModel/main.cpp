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
    // Initialize
}

void tst_AceTreeModel::basic() {
    AceTreeModel model;

    auto rootItem = new AceTreeItem("1");
    rootItem->setProperty("key1", "value1");

    // Step 1 - Test Change Root
    model.setRootItem(rootItem);
    model.previousStep();
    QVERIFY(!model.rootItem());
    model.nextStep();
    QCOMPARE(model.rootItem(), rootItem);

    // Step 2 - Test Set Property
    rootItem->setProperty("key1", "value2");
    model.previousStep();
    QCOMPARE(rootItem->property("key1"), "value1");
    model.nextStep();
    QCOMPARE(rootItem->property("key1"), "value2");
    model.previousStep();

    // Step 3 - Test Set Bytes
    rootItem->insertBytes(0, "ABCDEFGHIJKLMN");
    rootItem->setBytes(0, "1234");
    QCOMPARE(rootItem->bytes(), "1234EFGHIJKLMN");
    model.previousStep();
    QCOMPARE(rootItem->bytes(), "ABCDEFGHIJKLMN");
    rootItem->truncateBytes(4);
    QCOMPARE(rootItem->bytes(), "ABCD");
    model.previousStep();
    QCOMPARE(rootItem->bytes(), "ABCDEFGHIJKLMN");
    rootItem->setBytes(10, "123456");
    QCOMPARE(rootItem->bytes(), "ABCDEFGHIJ123456");
    model.previousStep();
    QCOMPARE(rootItem->bytes(), "ABCDEFGHIJKLMN");
    rootItem->removeBytes(2,2);
    QCOMPARE(rootItem->bytes(), "ABEFGHIJKLMN");
    model.previousStep();
    QCOMPARE(rootItem->bytes(), "ABCDEFGHIJKLMN");

    // Step 4 - Test insert and remove rows
    auto trackItem1 = new AceTreeItem("track1");
    auto trackItem2 = new AceTreeItem("track2");
    auto trackItem3 = new AceTreeItem("track3");
    auto trackItem4 = new AceTreeItem("track4");
    rootItem->appendRow(trackItem1);
    rootItem->appendRow(trackItem2);
    rootItem->appendRow(trackItem3);
    rootItem->appendRow(trackItem4);
    QCOMPARE(rootItem->rowCount(), 4);

    auto trackItem1_5 = new AceTreeItem("track5");
    rootItem->insertRow(1, trackItem1_5);
    {
        auto tmp = {trackItem1, trackItem1_5, trackItem2, trackItem3, trackItem4};
        QCOMPARE(rootItem->rows(), tmp);
    }
    model.previousStep();
    {
        auto tmp = {trackItem1, trackItem2, trackItem3, trackItem4};
        QCOMPARE(rootItem->rows(), tmp);
    }
    model.nextStep();
    {
        auto tmp = {trackItem1, trackItem1_5, trackItem2, trackItem3, trackItem4};
        QCOMPARE(rootItem->rows(), tmp);
    }
    rootItem->removeRows(1, 1);
    {
        auto tmp = {trackItem1, trackItem2, trackItem3, trackItem4};
        QCOMPARE(rootItem->rows(), tmp);
    }
    model.previousStep();
    {
        auto tmp = {trackItem1, trackItem1_5, trackItem2, trackItem3, trackItem4};
        QCOMPARE(rootItem->rows(), tmp);
    }
    model.nextStep();

    // Step 5 - Test move rows
    rootItem->moveRows(1, 2, 4);
    {
        auto tmp = {trackItem1, trackItem4, trackItem2, trackItem3};
        QCOMPARE(rootItem->rows(), tmp);
    }
    model.previousStep();
    {
        auto tmp = {trackItem1, trackItem2, trackItem3, trackItem4};
        QCOMPARE(rootItem->rows(), tmp);
    }

    // Step 6 - Test add records
    auto noteItem1 = new AceTreeItem("note1");
    auto noteItem2 = new AceTreeItem("note2");
    auto noteItem3 = new AceTreeItem("note3");
    auto noteItem4 = new AceTreeItem("note4");
    auto noteItem5 = new AceTreeItem("note5");

    int seq;
    seq = rootItem->addRecord(noteItem1);
    QCOMPARE(seq, 1);
    seq = rootItem->addRecord(noteItem2);
    QCOMPARE(seq, 2);
    seq = rootItem->addRecord(noteItem3);
    QCOMPARE(seq, 3);
    seq = rootItem->addRecord(noteItem4);
    QCOMPARE(seq, 4);
    seq = rootItem->addRecord(noteItem5);
    QCOMPARE(seq, 5);

    rootItem->removeRecord(2);
    {
        auto tmp = {1, 3, 4, 5};
        QCOMPARE(rootItem->records(), tmp);
    }
    model.previousStep();
    {
        auto tmp = {1, 2, 3, 4, 5};
        QCOMPARE(rootItem->records(), tmp);
    }
    model.nextStep();
}

QTEST_APPLESS_MAIN(tst_AceTreeModel)
#include "main.moc"