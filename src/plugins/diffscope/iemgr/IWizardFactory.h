#ifndef IWIZARDFACTORY_H
#define IWIZARDFACTORY_H

#include <QDateTime>
#include <QObject>

#include "IEMgrGlobal.h"

#include <CoreApi/IWindow.h>

namespace IEMgr {

    class IWizardFactoryPrivate;

    class IEMGR_EXPORT IWizardFactory : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IWizardFactory)
    public:
        explicit IWizardFactory(const QString &id, QObject *parent = nullptr);
        ~IWizardFactory();

        enum Feature {
            Import = 1,
            Export = 2,
        };
        Q_DECLARE_FLAGS(Features, Feature);

        virtual Features features() const = 0;
        virtual QString filter(Feature feature) const;
        virtual bool runWizard(Feature feature, const QString &path, Core::IWindow *windowHandle) = 0;

    public:
        QString id() const;

        QString iconPath() const;
        void setIconPath(const QString &path);

        QString description() const;
        void setDescription(const QString &description);

        QString displayName() const;
        void setDisplayName(const QString &displayName);

        QString category() const;
        void setCategory(const QString &category);

        QString displayCategory() const;
        void setDisplayCategory(const QString &displayCategory);

    protected:
        IWizardFactory(IWizardFactoryPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<IWizardFactoryPrivate> d_ptr;

        friend class IManager;
    };

}

#endif // IWIZARDFACTORY_H