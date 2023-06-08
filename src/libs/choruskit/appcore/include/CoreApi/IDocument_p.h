#ifndef IDOCUMENTPRIVATE_H
#define IDOCUMENTPRIVATE_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the ChorusKit API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QSettings>
#include <QSharedPointer>
#include <QTemporaryDir>

#include "DocumentSpec.h"
#include "IDocument.h"

namespace Core {

    class CKAPPCORE_API LogDirectory {
    public:
        explicit LogDirectory(const QString &prefix = {}) : prefix(prefix), autoRemove(true) {
        }

        ~LogDirectory();

        QString logDirectory() const;
        void setLogDirectory(const QString &dir);

        bool autoRemove;
        QString prefix;

    private:
        mutable QScopedPointer<QTemporaryDir> logDir;
        QString userLogDir;
    };

    class CKAPPCORE_API IDocumentPrivate {
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

    class CKAPPCORE_API IDocumentSettingsData : public QSharedData {
    public:
        QString dir;
        QSharedPointer<QSettings> settings;

        static inline QString descFile(const QString &dir) {
            return QString("%1/desc.tmp.ini").arg(dir);
        }
    };

}

#endif // IDOCUMENTPRIVATE_H