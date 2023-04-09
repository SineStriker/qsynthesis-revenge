#ifndef IMANAGER_H
#define IMANAGER_H

#include <QObject>

#include "IImportWizard.h"

namespace IEMgr {

    namespace Internal {
        class ImportManager;
    }

    class IManagerPrivate;

    class IEMGR_EXPORT IManager : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IManager)
    public:
        static IManager *instance();

    public:
        bool addImportWizard(IImportWizard *factory);
        bool removeImportWizard(IImportWizard *factory);
        bool removeImportWizard(const QString &id);
        QList<IImportWizard *> importWizards() const;
        void clearImportWizards();

        void runImport(const QString &id, QWidget *parent = nullptr);
        void runExport(const QString &id, QWidget *parent = nullptr);

        bool isRunning() const;

    private:
        explicit IManager(QObject *parent = nullptr);
        ~IManager();

        IManager(IManagerPrivate &d, QObject *parent = nullptr);

        QScopedPointer<IManagerPrivate> d_ptr;

        friend class Internal::ImportManager;
    };

}

#endif // IMANAGER_H