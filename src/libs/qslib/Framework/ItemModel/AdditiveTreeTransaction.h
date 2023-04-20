#ifndef ADDITIVETREETRANSACTION_H
#define ADDITIVETREETRANSACTION_H

#include <QUndoStack>

#include "AdditiveTreeModel.h"

namespace QsApi {

    class AdditiveTreeTransactionPrivate;

    class QSFRAMEWORK_API AdditiveTreeTransaction {
        Q_DECLARE_PRIVATE(AdditiveTreeTransaction)
    public:
        explicit AdditiveTreeTransaction(AdditiveTreeModel *model);
        ~AdditiveTreeTransaction();

    public:
        virtual void read(QDataStream &in);
        virtual void write(QDataStream &out) const;

        QVariant property(const QString &key) const;
        void setProperty(const QString &key, const QVariant &value);

    public:
        QUndoCommand *createCommand() const;

    private:
        QScopedPointer<AdditiveTreeTransactionPrivate> d_ptr;

        friend class AdditiveTreeItem;
        friend class AdditiveTreeModel;
    };

}

#endif // ADDITIVETREETRANSACTION_H