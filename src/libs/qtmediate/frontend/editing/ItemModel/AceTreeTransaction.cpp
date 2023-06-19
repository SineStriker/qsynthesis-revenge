#include "AceTreeTransaction.h"
#include "AceTreeRecoverable_p.h"

#include "QMSettingFile.h"

#include <QDebug>
#include <QPointer>
#include <QRegularExpression>
#include <QStack>
#include <QTextStream>

#define myWarning(func) (qWarning().nospace() << "AceTreeTransaction::" << (func) << "(): ").maybeSpace()

static const char Sign_Transaction[] = "Transaction";
static const char Sign_Operation[] = "Transaction";

class AceTreeTransactionPrivate : public AceTreeRecoverablePrivate {
    Q_DECLARE_PUBLIC(AceTreeTransaction)
public:
    AceTreeTransactionPrivate() {
        state = AceTreeTransaction::Idle;
        current = -1;
        stepIndex = 0;
        model = nullptr;
    }

    ~AceTreeTransactionPrivate() {
        if (model) {
            model->setProperty("ace_tree_transaction", QVariant());
        }
    }

    void init(AceTreeModel *m) {
        if (m->property("ace_tree_transaction").toBool()) {
            myWarning("AceTreeTransaction") << "A model cannot be managed by two transaction managers simultaneously";
            return;
        }
        m->setProperty("ace_tree_transaction", true);
        model = m;
    }

    QPointer<AceTreeModel> model;

    AceTreeTransaction::State state;

    struct Operation {
        QString key;
        QMap<QString, QString> args;

        int begin;
        int end;

        Operation() : begin(-1), end(0){};
        ~Operation() = default;

        bool isValid() const {
            return begin >= 0;
        }
    };

    int current;

    QStack<Operation> operations;
    int stepIndex;

    struct Offsets {
        qint64 startPos;
        qint64 countPos;
        qint64 totalPos;
        qint64 dataPos;
        QVector<qint64> begs;
        Offsets() : startPos(0), countPos(0), totalPos(0), dataPos(0) {
        }
    };
    Offsets offsets;

    void logStart() override {
        auto &dev = m_dev;

        QTextStream stream(dev);

        offsets.startPos = dev->pos();
        stream << "[" << Sign_Transaction << "]" << Qt::endl;

        stream << "Index=";
        offsets.countPos = dev->pos();
        writeFixLengthNumber(stream, stepIndex);
        stream << Qt::endl;

        stream << "Size=";
        offsets.totalPos = dev->pos();
        writeFixLengthNumber(stream, operations.size());
        stream << Qt::endl;

        offsets.dataPos = dev->pos();

        // Write all existing operations
        for (auto &operation : operations) {
            serializeOperation(stream, operation);

            if (stream.status() != QTextStream::Ok) {
                logError();
                return;
            }

            offsets.begs.append(dev->pos());
        }

        if (m_fileDev)
            m_fileDev->flush();
    }

    void logStop() override {
        offsets = {};
    }

    bool writeCurrentStep() const {
        qint64 pos = m_dev->pos();
        m_dev->seek(offsets.countPos);

        QTextStream stream(m_dev);
        writeFixLengthNumber(stream, stepIndex);

        m_dev->seek(offsets.totalPos);
        writeFixLengthNumber(stream, operations.size());

        m_dev->seek(pos);

        return stream.status() == QTextStream::Ok;
    }

    static void serializeOperation(QTextStream &stream, const Operation &op) {
        stream << "[" << Sign_Operation << " " << op.begin << "-" << (op.begin + op.end) << "]" << Qt::endl;
        stream << "%Formatter=" << op.key << Qt::endl;
        for (auto it = op.args.begin(); it != op.args.end(); ++it) {
            stream << it.key() << "=" << it.value() << Qt::endl;
        }
        stream << Qt::endl;
    }

    void push(const Operation &op) {
        truncate(stepIndex);

        operations.push(op);
        stepIndex++;

        // Serialize
        if (m_dev) {
            // Change step in log
            writeCurrentStep();

            QTextStream stream(m_dev);
            serializeOperation(stream, op);
            offsets.begs.append(m_dev->pos());

            if (stream.status() != QTextStream::Ok || (m_fileDev && (!m_fileDev->flush()))) {
                logError();
            }
        }
    }

    void truncate(int step) {
        operations.resize(step);
        offsets.begs.resize(step);
        stepIndex = step;

        // Serialize
        if (m_dev) {
            // Restore pos
            qint64 pos = offsets.begs.isEmpty() ? offsets.dataPos : offsets.begs.back();
            m_dev->seek(pos);
            if (!writeCurrentStep() || (m_fileDev && (!m_fileDev->resize(pos) || !m_fileDev->flush()))) {
                logError();
            }
        }
    }

    void writeCurrentStepAndFlush() {
        // Serialize
        if (m_dev) {
            // Change step in log
            if (!writeCurrentStep() || (m_fileDev && (!m_fileDev->flush()))) {
                logError();
            }
        }
    }

    static void writeFixLengthNumber(QTextStream &stream, int num) {
        stream << QString("%1").arg(num, 10, 10, QChar('0'));
    }
};

AceTreeTransaction::AceTreeTransaction(AceTreeModel *model, QObject *parent)
    : AceTreeTransaction(*new AceTreeTransactionPrivate(), model, parent) {
}

AceTreeTransaction::~AceTreeTransaction() {
}

bool AceTreeTransaction::isValid() const {
    Q_D(const AceTreeTransaction);
    return d->model != nullptr;
}

using DescriptionGetterFunc = QHash<QString, AceTreeTransaction::FormatDescription>;
Q_GLOBAL_STATIC(DescriptionGetterFunc, descGetterIndexes)

bool AceTreeTransaction::hasDescriptionFormatter(const QString &key) {
    return descGetterIndexes->contains(key);
}

void AceTreeTransaction::registerDescriptionFormatter(const QString &key, FormatDescription func) {
    if (key.isEmpty())
        return;

    if (!func) {
        descGetterIndexes->remove(key);
        return;
    }
    descGetterIndexes->insert(key, func);
}

AceTreeTransaction::State AceTreeTransaction::state() const {
    Q_D(const AceTreeTransaction);
    return d->state;
}

void AceTreeTransaction::beginTransaction() {
    Q_D(AceTreeTransaction);

    if (!isIdle()) {
        myWarning(__func__) << "Cannot start transaction, current state:" << state();
        return;
    }

    d->state = Transaction;

    // Save current step
    d->current = d->model->currentStep();
}

void AceTreeTransaction::abortTransaction() {
    Q_D(AceTreeTransaction);

    if (!inTransaction()) {
        myWarning(__func__) << "No transaction in execution";
        return;
    }

    // Abort
    d->model->setCurrentStep(d->current);
    d->model->truncateForwardSteps();
    d->current = -1;

    d->state = Idle;
}

void AceTreeTransaction::endTransaction(const QString &key, const QMap<QString, QString> &args) {
    if (!inTransaction()) {
        myWarning(__func__) << "No transaction in execution";
        return;
    }

    Q_D(AceTreeTransaction);

    AceTreeTransactionPrivate::Operation op;
    op.begin = d->current;
    op.end = d->model->currentStep();
    op.key = key;
    op.args = args;

    d->current = -1;

    // Commit
    d->push(op);

    d->state = Idle;

    emit indexChanged(index());
}

int AceTreeTransaction::count() const {
    Q_D(const AceTreeTransaction);
    return d->operations.size();
}

int AceTreeTransaction::index() const {
    Q_D(const AceTreeTransaction);
    return d->stepIndex;
}

QString AceTreeTransaction::description(int index) const {
    Q_D(const AceTreeTransaction);

    if (index >= 0 && index <= d->operations.size())
        return {};

    const auto &op = d->operations.at(index);
    auto func = descGetterIndexes->value(op.key);
    return func ? func(op.args) : QString();
}

void AceTreeTransaction::undo() {
    if (!isIdle()) {
        myWarning(__func__) << "Cannot executing undo, current state:" << state();
        return;
    }
    if (!canUndo()) {
        myWarning(__func__) << "No operation to undo";
        return;
    }

    Q_D(AceTreeTransaction);
    d->state = Undo;

    const auto &op = d->operations.at(d->stepIndex - 1);
    d->stepIndex--;
    d->model->setCurrentStep(op.begin);
    d->writeCurrentStepAndFlush();

    d->state = Idle;

    emit indexChanged(index());
}

void AceTreeTransaction::redo() {
    if (!isIdle()) {
        myWarning(__func__) << "Cannot executing redo, current state:" << state();
        return;
    }
    if (!canRedo()) {
        myWarning(__func__) << "No operation to undo";
        return;
    }

    Q_D(AceTreeTransaction);
    d->state = Redo;

    const auto &op = d->operations.at(d->stepIndex);
    d->stepIndex++;
    d->model->setCurrentStep(op.begin);
    d->writeCurrentStepAndFlush();

    d->state = Idle;

    emit indexChanged(index());
}

bool AceTreeTransaction::recover(const QByteArray &data) {
    Q_D(AceTreeTransaction);
    if (!d->operations.isEmpty()) {
        myWarning(__func__) << "cannot recover when transactions has changed";
        return false;
    }

    auto str = QString::fromUtf8(data);

    QTextStream stream(&str, QIODevice::ReadOnly);
    QStringList lines;
    while (!stream.atEnd()) {
        auto line = stream.readLine().trimmed();
        if (line.isEmpty())
            continue;
        lines << line;
    }

    QMSettingFile file;
    file.fromLines(lines);

    auto sections = file.sections();
    if (sections.isEmpty())
        return false;

    auto head = sections.takeFirst();
    if (head.sectionName() != Sign_Transaction) {
        return false;
    }

    int index, size;

    // Get index
    const QString *s;
    if (!(s = head.valueOf("Index"))) {
        return false;
    }
    index = s->toInt();

    // Get size
    if (!(s = head.valueOf("Size"))) {
        return false;
    }
    size = s->toInt();

    if (index > size || sections.size() < size)
        return false;
    while (sections.size() > size) {
        sections.pop_back();
    }

    decltype(d->operations) operations;
    QRegularExpression reg(QString(R"(%1(\d+)-(\d+))").arg(Sign_Operation));
    for (const auto &section : qAsConst(sections)) {
        QRegularExpressionMatch match = reg.match(section.sectionName());
        if (!match.hasMatch())
            return false;

        AceTreeTransactionPrivate::Operation op;
        op.begin = match.captured(1).toInt();
        op.end = match.captured(2).toInt();

        if ((s = section.valueOf("%Formatter"))) {
            op.key = *s;
            for (const auto &pair : section.toPairList()) {
                if (pair.first == "%Formatter")
                    continue;
                op.args.insert(pair.first, pair.second);
            }
        }
        operations << op;
    }

    if (operations.isEmpty())
        return true;

    int lastEnd = operations.front().end;
    for (int i = 1; i < operations.size(); ++i) {
        const auto &op = operations.at(i);
        if (op.begin != lastEnd || op.end <= op.begin)
            return false;
        lastEnd = op.end;
    }

    d->operations = std::move(operations);
    d->stepIndex = index;

    return true;
}

AceTreeTransaction::AceTreeTransaction(AceTreeTransactionPrivate &d, AceTreeModel *model, QObject *parent)
    : AceTreeRecoverable(d, parent) {
    d.init(model);
}
