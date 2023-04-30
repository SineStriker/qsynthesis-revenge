#ifndef IDOCUMENTPRIVATE_H
#define IDOCUMENTPRIVATE_H

#include <QTemporaryDir>

#include "DocumentSpec.h"
#include "IDocument.h"

namespace Core {

    class LogDirectory {
    public:
        explicit LogDirectory(const QString &prefix = {}) : prefix(prefix) {
        }

        ~LogDirectory();

        QString logDirectory() const;
        void setLogDirectory(const QString &dir);

        QString prefix;

    private:
        mutable QScopedPointer<QTemporaryDir> logDir;
        QString userLogDir;
    };

    class IDocumentPrivate {
        Q_DECLARE_PUBLIC(IDocument)
    public:
        IDocumentPrivate();
        virtual ~IDocumentPrivate();

        void init();

        bool getSpec();
        void updateLogDesc();

        IDocument *q_ptr;

        QString id;
        DocumentSpec *spec;

        mutable QString errMsg;

        QString mimeType;
        QString filePath;
        QString preferredDisplayName;
        QString uniqueDisplayName;
        QString autoSaveName;
        bool temporary;

        LogDirectory logDir;
    };

}

#endif // IDOCUMENTPRIVATE_H