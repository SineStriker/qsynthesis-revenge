#include <QDebug>
#include <QTest>

#include "MusicUtil/MusicTimeline.h"
#include "MusicUtil/MusicTimeline_p.h"

class tst_AceTreeModel : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void init();

private slots:
    void basic();
};

QsApi::MusicTimeline *timeline;

void tst_AceTreeModel::init() {
    // Initialize

    timeline = new QsApi::MusicTimeline();
}

class Timeline : public QsApi::MusicTimeline {
public:
    QsApi::MusicTimelinePrivate *get() const {
        return d_ptr.data();
    }
};

void tst_AceTreeModel::basic() {
    timeline->addTimeSignature(1, {3, 8});
    timeline->addTimeSignature(3, {5, 4});

    auto d = reinterpret_cast<Timeline *>(timeline)->get();

    qDebug() << "====Test====";
    qDebug() << d->timeSignatureMap.keys();
    qDebug() << d->timeSignatureMap.values();
    qDebug() << d->measureMap.keys();
    qDebug() << d->measureMap.values();
    timeline->addTimeSignature(2, {5, 4});
    qDebug() << "====Test====";
    qDebug() << d->timeSignatureMap.keys();
    qDebug() << d->timeSignatureMap.values();
    qDebug() << d->measureMap.keys();
    qDebug() << d->measureMap.values();
    timeline->removeTimeSignature(1);
    qDebug() << "====Test====";
    qDebug() << d->timeSignatureMap.keys();
    qDebug() << d->timeSignatureMap.values();
    qDebug() << d->measureMap.keys();
    qDebug() << d->measureMap.values();
    timeline->addTimeSignature(0, {3, 4});
    qDebug() << "====Test====";
    qDebug() << d->timeSignatureMap.keys();
    qDebug() << d->timeSignatureMap.values();
    qDebug() << d->measureMap.keys();
    qDebug() << d->measureMap.values();
    timeline->addTempo(2400, 90);
    timeline->addTempo(1200, 80);
    timeline->addTempo(1920, 60);
    qDebug() << "====Test2====";
    qDebug() << d->tempoMap.keys();
    qDebug() << d->tempoMap.values();
    qDebug() << d->msecSumMap.keys();
    qDebug() << d->msecSumMap.values();
    auto mt = timeline->create(0, 0, 5700);
    qDebug() << mt;
    mt = timeline->create(0, 0, 4000);
    qDebug() << mt;
    mt = timeline->create("3:3:160");
    qDebug() << mt;
    mt = timeline->create(3, 0, 420);
    qDebug() << mt;
    mt = timeline->create(2875);
    qDebug() << mt;
    mt = timeline->create(3000);
    qDebug() << mt;
    auto mt2 = mt;
    qDebug() << mt << mt2;
    timeline->removeTempos({2400, 1920, 1200});
    timeline->addTimeSignature(0, {4, 4});
    qDebug() << mt << mt2;
    qDebug() << "strconv:" << timeline->create("2");
    qDebug() << "strconv:" << timeline->create("2:");
    qDebug() << "strconv:" << timeline->create(":2");
    qDebug() << "strconv:" << timeline->create("2:2:");
    qDebug() << "strconv:" << timeline->create("::10");
    qDebug() << "strconv:" << timeline->create("2:0:0");
    mt += 960;
    qDebug() << mt;
    qDebug() << (mt == mt2);
}

QTEST_APPLESS_MAIN(tst_AceTreeModel)

#include "main.moc"
