#ifndef IMANAGER_H
#define IMANAGER_H

#include <QObject>

#include "IWizardFactory.h"

namespace IEMgr {

    namespace Internal {
        class IEMgrPlugin;
    }

    class IManagerPrivate;

    class IEMGR_EXPORT IManager : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IManager)
    public:
        static IManager *instance();

    public:
        bool addWizard(IWizardFactory *factory);
        bool removeWizard(IWizardFactory *factory);
        bool removeWizard(const QString &id);
        QList<IWizardFactory *> wizards() const;
        void clearWizards();

    public:
        void runImport(QWidget *parent, const QString &wizardId = {}, const QString &path = {});
        void runExport(QWidget *parent, const QString &wizardId = {}, const QString &path = {});

        bool isRunning() const;

    private:
        explicit IManager(QObject *parent = nullptr);
        ~IManager();

        IManager(IManagerPrivate &d, QObject *parent = nullptr);

        QScopedPointer<IManagerPrivate> d_ptr;

        friend class Internal::IEMgrPlugin;
    };

}

#endif // IMANAGER_H