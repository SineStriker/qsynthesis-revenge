#ifndef DSPXHISTORY_H
#define DSPXHISTORY_H

#include <AceTreeModel.h>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class CORE_EXPORT DspxOperator {
    public:
        using GetText = std::function<QString(const QHash<QString, QString> &)>;

        DspxOperator();
        DspxOperator(const QString &name, const QString &source, const GetText &textGetter);

        QString name() const;
        void setName(const QString &name);

        QString source() const;
        void setSource(const QString &source);

        GetText textGetter() const;
        void setTextGetter(const GetText &textGetter);

    private:
        QString m_name;
        QString m_source;
        GetText m_textGetter;
    };

    class DspxDocument;

    class DspxDocumentPrivate;

    class DspxHistoryData;

    class CORE_EXPORT DspxHistory {
        Q_DISABLE_COPY_MOVE(DspxHistory)
    public:
        void beginTransaction();
        void abortTransaction();
        void commitTransaction(const QString &opId, const QHash<QString, QString> &attributes);

        int minStep() const;
        int maxStep() const;
        int currentStep() const;
        QHash<QString, QString> stepAttributes(int step) const;

        AceTreeModel::State state() const;

        QString undoText() const;
        QString redoText() const;

        void undo();
        void redo();

        inline bool canUndo() const;
        inline bool canRedo() const;

    public:
        static void registerOperator(const QString &key, const DspxOperator &op);
        static void removeOperator(const QString &key);

    private:
        DspxHistory(DspxDocumentPrivate *d);

        DspxHistoryData *d;

        friend class DspxDocument;
        friend class DspxDocumentPrivate;
    };

    inline bool DspxHistory::canUndo() const {
        return currentStep() > minStep();
    }

    inline bool DspxHistory::canRedo() const {
        return currentStep() < maxStep();
    }

}



#endif // DSPXHISTORY_H
