#ifndef IWIZARDFACTORY_H
#define IWIZARDFACTORY_H

#include <QDateTime>
#include <QObject>

#include <Text/QMDisplayString.h>

#include "IEMgrGlobal.h"

namespace IEMgr {

    class IWizardFactoryPrivate;

    class IEMGR_EXPORT IWizardFactory : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IWizardFactory)
    public:
        explicit IWizardFactory(const QString &id, QObject *parent = nullptr);
        ~IWizardFactory();

        enum Feature {
            ImportProject = 1,
            ExportProject = 2,
            ImportAudio = 4,
            ExportAudio = 8,
        };
        Q_ENUM(Feature)
        Q_DECLARE_FLAGS(Features, Feature)

        virtual Features features() const = 0;
        virtual QString filter(Feature feature) const;
        virtual bool runWizard(Feature feature, const QString &path, QWidget *parent) = 0;

    public:
        QString id() const;

        QString iconPath() const;
        void setIconPath(const QString &path);

        QMDisplayString description() const;
        void setDescription(const QMDisplayString &description);

        QMDisplayString displayName() const;
        void setDisplayName(const QMDisplayString &displayName);

        QString category() const;
        void setCategory(const QString &category);

        QMDisplayString displayCategory() const;
        void setDisplayCategory(const QMDisplayString &displayCategory);

    protected:
        IWizardFactory(IWizardFactoryPrivate &d, const QString &id, QObject *parent = nullptr);

        QScopedPointer<IWizardFactoryPrivate> d_ptr;

        friend class IManager;
    };

}

#endif // IWIZARDFACTORY_H