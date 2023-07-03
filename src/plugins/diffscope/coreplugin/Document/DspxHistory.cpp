#include "DspxHistory.h"

#include "DspxDocument_p.h"

namespace Core {

    using OperatorIndexes = QHash<QString, DspxOperator>;
    Q_GLOBAL_STATIC(OperatorIndexes, m_ops);

    DspxOperator::DspxOperator() {
    }
    DspxOperator::DspxOperator(const QString &name, const QString &source,
                               const Core::DspxOperator::GetText &textGetter)
        : m_name(name), m_source(source), m_textGetter(textGetter) {
    }
    QString DspxOperator::name() const {
        return m_name;
    }
    void DspxOperator::setName(const QString &name) {
        m_name = name;
    }
    QString DspxOperator::source() const {
        return m_source;
    }
    void DspxOperator::setSource(const QString &source) {
        m_source = source;
    }
    DspxOperator::GetText DspxOperator::textGetter() const {
        return m_textGetter;
    }
    void DspxOperator::setTextGetter(const Core::DspxOperator::GetText &textGetter) {
        m_textGetter = textGetter;
    }

    void DspxHistory::beginTransaction() {
        d->dp->model->beginTransaction();
    }

    void DspxHistory::abortTransaction() {
        d->dp->model->abortTransaction();
    }

    void DspxHistory::commitTransaction(const QString &opId, const QHash<QString, QString> &attributes) {
        auto attrs = attributes;
        attrs.insert("op_id", opId);
        d->dp->model->commitTransaction(attrs);
    }

    int DspxHistory::minStep() const {
        return d->dp->model->minStep();
    }

    int DspxHistory::maxStep() const {
        return d->dp->model->maxStep();
    }

    int DspxHistory::currentStep() const {
        return d->dp->model->currentStep();
    }

    QHash<QString, QString> DspxHistory::stepAttributes(int step) const {
        auto res = d->dp->model->stepAttributes(step);
        res.remove("op_id");
        return res;
    }

    AceTreeModel::State DspxHistory::state() const {
        return d->dp->model->state();
    }

    static QString getStepText(QHash<QString, QString> &attrs) {
        if (attrs.isEmpty())
            return {};

        auto it0 = attrs.find("op_id");
        if (it0 == attrs.end())
            return {};

        auto it = m_ops->find(*it0);
        if (it == m_ops->end())
            return {};

        attrs.erase(it0);

        return it.value().textGetter()(attrs);
    }

    QString DspxHistory::undoText() const {
        auto model = d->dp->model;
        auto attrs = model->stepAttributes(model->currentStep() - 1);
        return getStepText(attrs);
    }

    QString DspxHistory::redoText() const {
        auto model = d->dp->model;
        auto attrs = model->stepAttributes(model->currentStep());
        return getStepText(attrs);
    }

    void DspxHistory::undo() {
        d->dp->model->previousStep();
    }

    void DspxHistory::redo() {
        d->dp->model->nextStep();
    }

    void DspxHistory::registerOperator(const QString &key, const DspxOperator &op) {
        m_ops->insert(key, op);
    }

    void DspxHistory::removeOperator(const QString &key) {
        m_ops->remove(key);
    }

    DspxHistory::DspxHistory(DspxDocumentPrivate *d) : d(new DspxHistoryData(d)) {
    }

}
